/*
 * A198970  a(n) = 4*10^n - 1.
 * https://oeis.org/A198970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*10^n - 1 (recurrence: a(0)=3, a(n)=10*a(n-1)+9 for n>0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

/* Compute term a(n) of OEIS A198970 */
seq_t a(idx_t n)
{
    /* Start with a(0) = 3 */
    seq_t value = (seq_t)3;

    /* Apply the recurrence a(i)=10*a(i-1)+9 for i=1..n */
    for (idx_t i = 1; i <= n; ++i)
    {
        value = mul_small(value, 10);  /* value *= 10 */
        value += (seq_t)9;             /* value += 9   */
    }

    return value;
}
