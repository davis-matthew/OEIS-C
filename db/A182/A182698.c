/*
 * A182698  a(n) is the largest 5-digit number with exactly n divisors, or
 *          a(n) = 0 if no such number exists.
 * https://oeis.org/A182698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 128, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the largest 5‑digit integer with exactly n divisors, or 0 if none. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* static cache: best[n] = largest 5‑digit number with n divisors, 0 if none */
    static bool initialized = false;
    static seq_t best[129];

    if (!initialized) {
        /* initialize entries to 0 */
        for (size_t i = 0; i < 129; ++i) {
            best[i] = 0;
        }
        /* scan all 5‑digit numbers */
        for (idx_t x = 10000; x <= 99999; ++x) {
            uint64_t d = divisor_count_u64((uint64_t)x);
            if (d <= 128U) {
                size_t idx = (size_t)d;
                seq_t sx = (seq_t)x;
                if (best[idx] < sx) {
                    best[idx] = sx;
                }
            }
        }
        initialized = true;
    }

    if (n <= 128U) {
        return best[(size_t)n];
    }
    return (seq_t)0;
}
