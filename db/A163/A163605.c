/*
 * A163605  a(n) = ((3+2*sqrt(2))*(5+sqrt(2))^n +
 *          (3-2*sqrt(2))*(5-sqrt(2))^n)/2.
 * https://oeis.org/A163605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) for n > 1; initial values: a(0) =
 *            3, a(1) = 19
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   mul_small_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mul_small_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)19;

    seq_t prev2 = 3;   /* a(0) */
    seq_t prev1 = 19;  /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t t1 = mul_small_seq(prev1, 10); /* 10 * a(n-1) */
        seq_t t2 = mul_small_seq(prev2, 23); /* 23 * a(n-2) */
        current = t1 - t2;                   /* recurrence */
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
