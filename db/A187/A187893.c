/*
 * A187893  a(0)=1, a(1)=4, a(n) = a(n-1) + a(n-2) - 1.
 * https://oeis.org/A187893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(n) = a(n-1)+a(n-2)-1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Two‑term recurrence */
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)4;  /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2 - (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
