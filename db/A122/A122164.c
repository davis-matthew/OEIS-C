/*
 * A122164  a(0) = 0, a(1) = 1, s = 0; for n >= 2, if a(n-1) is even and s = 0
 *          then set a(n) = a(n-1)/2 and s = 1; otherwise set a(n) = a(n-1) +
 *          a(n-2) and s = 0.
 * https://oeis.org/A122164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 100, u64 n <= 202,
 *            u128 n <= 404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 0, a(1) = 1.
   For n ≥ 2, let s be 0 initially.
   If a(n‑1) is even and s == 0 then a(n) = a(n‑1)/2, s = 1;
   otherwise a(n) = a(n‑1)+a(n‑2), s = 0. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* a(n‑2) and a(n‑1) for the recurrence. */
    seq_t prev2 = 0;   /* a(0)          */
    seq_t prev1 = 1;   /* a(1)          */

    /* The flag s (0 = allow division, 1 = already divided). */
    bool s = 0;

    seq_t curr;
    for (idx_t i = 2; i <= n; ++i) {
        /* Even test: (prev1 & 1) == 0 checks if the least significant
           bit is 0, i.e., prev1 is even. */
        if (((prev1 & 1) == 0) && !s) {
            curr = prev1 / 2;
            s = true;
        } else {
            curr = prev1 + prev2;
            s = false;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
