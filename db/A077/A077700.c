/*
 * A077700  a(1) = 4; a(n) is the smallest multiple of a(n-1) that contains
 *          all the digits of a(n-1) and is not a multiple of 10.
 * https://oeis.org/A077700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=4; for n>1, a(n) is the smallest multiple of a(n‑1) that *
 *            contains every digit of a(n‑1) at least as many times and is not
 *            a * multiple of 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* safety for out of range indices */
    seq_t prev = 4;                 /* a(1) */
    if (n == 1) return prev;

    for (idx_t i = 2; i <= n; ++i) {
        /* Digit counts of the previous term */
        unsigned char prevCnt[10] = {0};
        digit_counts(prev, prevCnt);

        idx_t k = 2;
        for (;;) {
            seq_t cand = prev * (seq_t)k;
            /* Reject multiples ending with 0 */
            if (cand % 10 == 0) { ++k; continue; }

            /* Digit counts of the candidate */
            unsigned char candCnt[10] = {0};
            digit_counts(cand, candCnt);

            /* Check that every digit occurs at least as often as in prev */
            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (prevCnt[d] > candCnt[d]) { ok = false; break; }
            }
            if (ok) {
                prev = cand;
                break;
            }
            ++k;
        }
    }
    return prev;
}
