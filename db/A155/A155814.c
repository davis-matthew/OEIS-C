/*
 * A155814  Numbers n with the property that all four polynomials mentioned in
 *          A139414 evaluate at n to composite numbers.
 * https://oeis.org/A155814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer n for which the four quadratic
 *            polynomials p1(n) = 4*n^2 - 146*n + 1373 p2(n) = 4*n^2 - 144*n +
 *            1459 p3(n) = 4*n^2 - 142*n + 1301 p4(n) = 4*n^2 - 140*n + 1877
 *            all yield composite values. The function returns the a(n) for n
 *            ≥ 1. It caches previously found terms so...
 * Range:     u8 none, u16 n <= 2, u32 n <= 8254, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, never called */

    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;        /* number of stored terms          */
    static size_t cache_cap  = 0;        /* allocated capacity               */
    static idx_t  next_cand  = 1;        /* next integer to test            */

    /* Grow cache if needed */
    if (n > cache_len) {
        /* Ensure storage */
        if (!cache) {
            cache_cap = 128;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (n > cache_cap) {
            while (n > cache_cap)
                cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Generate terms until we have at least n of them */
        while (cache_len < (size_t)n) {
            seq_t nn = (seq_t)next_cand;

            seq_t v1 = 4 * nn * nn - 146 * nn + 1373;
            seq_t v2 = 4 * nn * nn - 144 * nn + 1459;
            seq_t v3 = 4 * nn * nn - 142 * nn + 1301;
            seq_t v4 = 4 * nn * nn - 140 * nn + 1877;

            if (isPrimeGeneric(v1) || isPrimeGeneric(v2) ||
                isPrimeGeneric(v3) || isPrimeGeneric(v4)) {
                /* Not valid, skip */
            } else {
                cache[cache_len++] = nn;
            }
            ++next_cand;
        }
    }

    return cache[n - 1];
}
