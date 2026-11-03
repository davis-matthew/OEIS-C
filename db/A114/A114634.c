/*
 * A114634  Numbers k such that the k-th octagonal number is 6-almost prime.
 * https://oeis.org/A114634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k for the n-th integer k with
 *            bigomega(k)+bigomega(3k‑2)==6 (that is, k such that k·(3k‑2) has
 *            exactly six prime factors, counting multiplicity)
 * Range:     u8 n <= 42, u16 n <= 8719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9214 (2026-11-01)
 * Helpers:   bigomega_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a direct search; it is fast enough to
   compute any term up to n≈10000 within the required limits. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega_u64.h"

seq_t a(idx_t n)
{
    /* The search finds the n-th qualifying integer k. */
    uint64_t k = 1;          /* candidate k */
    idx_t    found = 0;      /* how many have been found so far */
    seq_t    result = 0;     /* the desired term */

    while (true) {
        uint64_t val3k2 = 3u * k - 2u;            /* 3*k-2, fits in 64 bits for typical n */
        seq_t    sum   = bigomega_u64(k) + bigomega_u64(val3k2);

        if (sum == 6) {                           /* qualifies? */
            ++found;
            if (found == n) {                     /* the nth term found */
                result = (seq_t)k;
                break;
            }
        }
        ++k;                                      /* try next candidate */
    }

    return result;
}
