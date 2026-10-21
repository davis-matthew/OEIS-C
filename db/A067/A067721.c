/*
 * A067721  Least number k such that k (k + n) is a perfect square, or 0 if
 *          impossible.
 * https://oeis.org/A067721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k with k*(k+n) a perfect square.
   For each divisor d of n set n = d * y.  For every factor pair
   a*b = y, if a and b have the same parity let u = (b-a)/2 > 0.
   Then k = d*u^2 is a candidate; keep the minimum over all
   candidates.  If no candidate exists return 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_uint64.h"

seq_t a(idx_t n)
{
    /* Special case from the OEIS entry: n == 0 ⇒ k = 1. */
    if (n == 0)
        return 1;

    /* --- Helper data for divisor enumeration ---------------------------------
       Reuse the buffers across calls to avoid repeated allocation.
       ndivs  - divisors of n
       ydivs  - divisors of y = n/d
    --------------------------------------------------------------------------- */
    static uint64_t *ndivs = NULL;
    static size_t ncap = 0, nlen = 0;
    static uint64_t *ydivs = NULL;
    static size_t ycap = 0, ylen = 0;

    /* Buffers for prime factorisation (max 20 distinct primes for n≤2^64).  */
    uint64_t fac[20];
    int exp[20];
    int factcnt;

    /* Enumerate divisors of n. */
    {
        factor_uint64(n, fac, exp, &factcnt);

        if (ndivs == NULL || ncap < 256) {
            ncap = 256;
            ndivs = (uint64_t *)realloc(ndivs, ncap * sizeof(uint64_t));
        }
        ndivs[0] = 1;
        nlen = 1;

        for (int pi = 0; pi < factcnt; ++pi) {
            size_t oldlen = nlen;
            uint64_t p = fac[pi];
            uint64_t p_pow = 1;
            for (int ei = 1; ei <= exp[pi]; ++ei) {
                p_pow *= p;
                for (size_t j = 0; j < oldlen; ++j) {
                    if (nlen == ncap) {
                        ncap <<= 1;
                        ndivs = (uint64_t *)realloc(ndivs, ncap * sizeof(uint64_t));
                    }
                    ndivs[nlen++] = ndivs[j] * p_pow;
                }
            }
        }
    }

    seq_t best = 0;   /* 0 means "no solution found yet". */

    /* Loop over each divisor d of n. */
    for (size_t idx = 0; idx < nlen; ++idx) {
        uint64_t d = ndivs[idx];
        uint64_t y = n / d;
        if (y == 0) continue;   /* shouldn't happen */

        /* Enumerate divisors of y. */
        {
            factor_uint64(y, fac, exp, &factcnt);

            if (ydivs == NULL || ycap < 256) {
                ycap = 256;
                ydivs = (uint64_t *)realloc(ydivs, ycap * sizeof(uint64_t));
            }
            ydivs[0] = 1;
            ylen = 1;

            for (int pi = 0; pi < factcnt; ++pi) {
                size_t oldlen = ylen;
                uint64_t p = fac[pi];
                uint64_t p_pow = 1;
                for (int ei = 1; ei <= exp[pi]; ++ei) {
                    p_pow *= p;
                    for (size_t j = 0; j < oldlen; ++j) {
                        if (ylen == ycap) {
                            ycap <<= 1;
                            ydivs = (uint64_t *)realloc(ydivs, ycap * sizeof(uint64_t));
                        }
                        ydivs[ylen++] = ydivs[j] * p_pow;
                    }
                }
            }
        }

        /* Check each factor pair (a,b) of y. */
        for (size_t j = 0; j < ylen; ++j) {
            uint64_t a_factor = ydivs[j];
            uint64_t b_factor = y / a_factor;
            if (a_factor > b_factor) continue;                /* enforce a ≤ b */
            if ((((a_factor + b_factor) & 1ULL) != 0ULL)) continue; /* parity must match */

            uint64_t u = (b_factor - a_factor) / 2;
            if (u == 0) continue;   /* u must be positive */

            seq_t cand = (seq_t)d * (seq_t)u * (seq_t)u;
            if (best == 0 || cand < best) best = cand;
        }
    }

    return best;
}
