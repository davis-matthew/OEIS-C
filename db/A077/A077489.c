/*
 * A077489  a(n) = smallest multiple of 4 with sum of digits = n.
 * https://oeis.org/A077489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   minimal_value_with_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: minimal multiple of 4 with digit sum n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/minimal_value_with_sum.h"

seq_t a(idx_t n)
{
    /* Precompute minimal two‑digit multiples of 4 for each digit sum. */
    static uint8_t minTwo[19];
    static bool init = false;
    if (!init) {
        init = true;
        for (int s = 0; s < 19; ++s) minTwo[s] = 100;   /* sentinel > 99 */
        for (int d = 0; d < 100; d += 4) {               /* multiples of 4 */
            int s = d / 10 + d % 10;                    /* digit sum */
            if (d < minTwo[s]) minTwo[s] = d;
        }
    }

    /* Special case: single‑digit numbers. */
    if (n <= 9 && n % 4 == 0) return (seq_t)n;

    /* Minimal possible length of the number. */
    idx_t L = (n + 8) / 9;          /* ceil(n/9) */
    if (L < 2) L = 2;               /* because n>9 or not divisible by 4 */

    for (;; ++L) {
        if (L == 2) {
            uint8_t d = minTwo[(int)n];
            if (d < 100) return (seq_t)d;
            continue;
        }

        /* L >= 3 */
        idx_t m = L - 2;            /* digits before the last two */
        seq_t best = 0;
        bool found = false;

        for (int s = 0; s <= 18; ++s) {
            uint8_t d = minTwo[s];
            if (d == 100) continue;                /* no two‑digit number with this sum */
            if ((idx_t)s > n) continue;            /* would make remaining sum negative */
            idx_t rem = n - s;                     /* digits sum needed for the high part */
            if (rem > 9 * m) continue;

            seq_t high = minimal_value_with_sum(m, rem);
            seq_t candidate = high * 100 + d;
            if (!found || candidate < best) {
                best = candidate;
                found = true;
            }
        }

        if (found) return best;
    }
}
