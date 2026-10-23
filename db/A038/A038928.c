/*
 * A038928  Primes p such that 47 is not a square mod p.
 * https://oeis.org/A038928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038928: primes p such that 47 is not a quadratic residue modulo p. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Returns the nth term of the sequence (1‑based indexing). */
    if (n == 0) return 0;          /* undefined offset – safety value */

    static seq_t *cache      = NULL; /* array of collected terms            */
    static size_t cache_cap  = 0;    /* current capacity of cache          */
    static size_t cache_cnt  = 0;    /* number of terms stored already     */
    static idx_t  candidate  = 3;    /* next odd candidate (start at 3)    */

    while (cache_cnt < (size_t)n)
    {
        if (is_prime(candidate))
        {
            int leg = jacobi_symbol(47ULL, candidate);
            if (leg == -1)            /* 47 is a non‑residue modulo candidate */
            {
                if (cache_cnt >= cache_cap)
                {
                    size_t newcap = (cache_cap == 0) ? 4 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache      = tmp;
                    cache_cap  = newcap;
                }
                cache[cache_cnt++] = (seq_t)candidate;
            }
        }
        candidate += 2;              /* skip even numbers */
    }

    return cache[n - 1];
}
