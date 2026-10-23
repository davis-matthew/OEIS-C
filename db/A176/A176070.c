/*
 * A176070  Numbers of the form k^3+k^2+k+1 that are the product of two
 *          distinct primes.
 * https://oeis.org/A176070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k+1) * (k²+1) where k is the n‑th even integer ≥ 2 such
 *            that both k+1 and k²+1 are prime. The sequence contains those
 *            products (product of two distinct primes)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th term of the sequence, for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Return 0 for the unused 0‑th argument. */
    if (n == 0)
        return (seq_t)0;

    /* --------------------------------------------------------------------
       Cached terms: the array grows as needed.                            */
    static seq_t *cache = NULL;     /* array of known terms            */
    static size_t cap   = 0;        /* allocated capacity              */
    static idx_t   size  = 0;       /* number of stored terms          */
    static idx_t   next_k = 2;      /* next even k to try (k ≥ 2)      */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        idx_t k = next_k;
        next_k += 2;                /* next even integer */

        idx_t kp1  = k + 1;              /* k+1  */
        idx_t ksq1 = k * k + 1;          /* k²+1 */

        if (is_prime(kp1) && is_prime(ksq1)) {
            /* Grow the cache if necessary. */
            if (size == cap) {
                size_t new_cap = (cap == 0) ? 4 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* memory allocation failed */
                cache = tmp;
                cap   = new_cap;
            }

            /* Compute the product.  For arbitrary‑precision seq_t the
               multiplication uses the type's operator*; for unsigned
               integer types the usual arithmetic applies. */
            seq_t prod = (seq_t)kp1 * (seq_t)ksq1;
            cache[size++] = prod;
        }
    }

    return cache[n - 1];
}
