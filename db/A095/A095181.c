/*
 * A095181  Triangle read by rows in which the j-th row begins with prime(j)
 *          and is the arithmetic progression with least common difference
 *          such that the remaining j-1 terms are composite and not divisible
 *          by prime(j).
 * https://oeis.org/A095181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Build the triangle row by row. For the j‑th row the first
 *            element is the j‑th prime. Find the smallest positive difference
 *            d such that the arithmetic progression p, p+d, p+2d, … ,
 *            p+(j-1)d has the first term prime and all subsequent terms
 *            composite and not divisible by p. All rows are concatenate...
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes required for static allocation and booleans */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>

/* Utility functions supplied by the build environment */
#include "../utils/get_prime.h"      /* forward: uint32_t get_prime(idx_t k);   */
#include "../utils/is_prime_big.h"   /* forward: bool is_prime_big(seq_t n);    */

/* Function to compute the n-th term of A095181 */
seq_t a(idx_t n)
{
    /* Static cache for all computed terms */
    static seq_t *cache   = NULL;   /* array of terms   */
    static size_t cache_size = 0;   /* number of terms stored  */
    static size_t cache_cap  = 0;   /* allocated capacity      */
    static idx_t   next_row = 1;    /* next j to generate      */

    /* Generate rows until we have at least n terms */
    while (cache_size < n)
    {
        idx_t j = next_row;                     /* row length              */
        seq_t p = (seq_t)get_prime(j);          /* first term of row j     */
        seq_t d = 1;                            /* trial common difference */

        /* Search for the minimal d  */
        for (;; ++d)
        {
            bool ok = true;
            /* test all remaining j-1 terms */
            for (idx_t m = 1; m < j; ++m)
            {
                seq_t term = p + (seq_t)m * d;   /* arithmetic progression */
                if (term % p == 0) {            /* divisible by the prime   */
                    ok = false;
                    break;
                }
                if (is_prime_big(term)) {        /* term is still prime     */
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;                         /* suitable d found        */
        }

        /* Append this row's terms to the cache */
        for (idx_t k = 0; k < j; ++k)
        {
            seq_t val = p + (seq_t)k * d;

            /* Ensure enough storage for the new term */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)                      /* allocation failed  */
                    return 0;                  /* abort gracefully   */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;         /* store the term      */
        }
        ++next_row;                            /* move to next row    */
    }

    return cache[n - 1];    /* zero‑based indexing into the cache */
}
