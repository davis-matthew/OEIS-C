/*
 * A082414  a(n) = (2*10^n + 4^n)/3.
 * https://oeis.org/A082414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*10^n + 4^n)/3 Recurrence: a(0) = 1, a(1) = 8, a(n) =
 *            14*a(n-1) - 40*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)8;          /* a(1) */
    seq_t curr  = prev1;            /* placeholder */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        seq_t t1 = mul_small(prev1, 14);   /* 14*a(n-1) */
        seq_t t2 = mul_small(prev2, 40);   /* 40*a(n-2) */
        curr   = t1 - t2;                  /* recurrence */
        prev2 = prev1;                     /* shift for next iteration */
        prev1 = curr;
    }

    return prev1;
}
