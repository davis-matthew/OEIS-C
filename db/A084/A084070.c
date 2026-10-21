/*
 * A084070  a(n) = 38*a(n-1) - a(n-2), with a(0)=0, a(1)=6.
 * https://oeis.org/A084070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 38*a(n‑1) – a(n‑2) (n ≥ 2), a(0) = 0, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a simple iterative recurrence – no dynamic memory is needed. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    seq_t prev2 = (seq_t)0;          /* a(0) */
    seq_t prev1 = (seq_t)6;          /* a(1) */
    seq_t cur = prev1;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)38 - prev2;  /* a(i) = 38*a(i‑1) – a(i‑2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;      /* a(n) */
}
