/*
 * A015013  q-factorial numbers for q=-2.
 * https://oeis.org/A015013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{k=1..n} ((-2)^k - 1) / (-3), a(0) = 1 Recurrence used:
 *            a(n) = (((-2)^n - 1) * a(n‑1)) / (-3)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t ans   = 1;          /* a(0) */
    seq_t power = 1;          /* (-2)^0 */

    for (idx_t i = 1; i <= n; ++i) {
        power = power * (seq_t)(-2);       /* (-2)^i */
        seq_t term = power - 1;            /* (-2)^i - 1 */
        ans = (term * ans) / (seq_t)(-3);  /* exact division by -3 */
    }

    return ans;
}
