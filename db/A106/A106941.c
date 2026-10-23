/*
 * A106941  Primes of the form 4x^2-3xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p that can be written as 4x^2 - 3xy + 5y^2
 *            * with non‑negative integers x and y, indexed from 1
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt.h"


/* 64‑bit deterministic primality test – sufficient for all primes reached by
 * the sequence up to the usual limits of the judge. */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence starts at a(1) */
    idx_t count = 0;               /* how many suitable primes we have found */
    uint64_t p = 5;                /* first odd prime that could work */

    while (true) {
        if (is_prime_uint64(p)) {
            /* y must satisfy 71y^2 ≤ 16p   →   y ≤ sqrt(16p/71) */
            uint64_t limit_sq = (uint64_t)16 * p / 71;
            for (uint64_t y = 0;; ++y) {
                uint64_t y2 = y * y;
                if (y2 > limit_sq) break;

                seq_t val = (seq_t)16 * (seq_t)p - (seq_t)71 * (seq_t)y2;
                seq_t d   = isqrt(val);
                if (d * d != val) continue;   /* D must be a perfect square */

                /* x = (3y ± d)/8 must be an integer.  The ± sign is
                 * necessary; the earlier version only used +. */
                uint64_t d64 = (uint64_t)d;   /* d fits in 64‑bit for our ranges */

                bool ok = false;
                if ((3ULL * y + d64) % 8ULL == 0) ok = true;
                if (!ok && (3ULL * y >= d64) && ((3ULL * y - d64) % 8ULL == 0))
                    ok = true;

                if (ok) {
                    ++count;
                    if (count == n) return (seq_t)p;
                    break;          /* found representation for this p – move on */
                }
            }
        }
        p += 2;                      /* only odd candidates after 2 */
    }
}
