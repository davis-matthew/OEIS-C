/*
 * A071360  Smallest n-digit prime with strictly decreasing digits.
 * https://oeis.org/A071360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the smallest n‑digit prime with strictly decreasing digits */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime64.h"

seq_t a(idx_t n)
{
    /* Cache for the 10 possible values (digits 1–10). 0 means no prime. */
    static seq_t cache[11];
    static bool init = false;

    if (!init) {
        /* Precompute once */
        for (unsigned k = 1; k <= 10; ++k) {
            seq_t best = 0;                     /* 0 indicates not found yet */
            for (unsigned mask = 1; mask < (1U << 10); ++mask) {
                if (__builtin_popcount(mask) != k)
                    continue;

                /* Highest digit (most significant) must be non‑zero */
                int highest = 0;
                for (int d = 9; d >= 0; --d)
                    if (mask & (1U << d)) { highest = d; break; }

                if (highest == 0)
                    continue;

                /* Build the number from digits 9→0 */
                uint64_t num = 0;
                for (int d = 9; d >= 0; --d)
                    if (mask & (1U << d))
                        num = num * 10 + d;

                if (is_prime64(num)) {
                    seq_t val = (seq_t)num;
                    if (best == 0 || val < best)
                        best = val;
                }
            }
            cache[k] = best;
        }
        init = true;
    }

    if (n > 10)
        return (seq_t)0;
    return cache[(unsigned)n];
}
