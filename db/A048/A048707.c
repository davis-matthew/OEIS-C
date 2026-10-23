/*
 * A048707  Numerators of ratios converging to Thue-Morse constant.
 * https://oeis.org/A048707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (a(n-1)+1)*((2^(2^(n-1)))-1), with a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute A048707: Numerators of ratios converging to Thue-Morse constant. */
seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t acc = (seq_t)0;          /* accumulator for a(n-1)+1 term   */
    seq_t pow2 = (seq_t)2;         /* 2^(2^(i-1)) for the current i   */

    for (idx_t i = 1; i <= n; ++i) {
        /* factor = 2^(2^(i-1)) - 1  */
        seq_t factor = pow2 - (seq_t)1;

        /* recurrence step: a(i) = (a(i-1)+1) * factor */
        acc = (acc + (seq_t)1) * factor;

        /* prepare pow2 for the next iteration:
           2^(2^i) = (2^(2^(i-1)))^2
        */
        pow2 = pow2 * pow2;
    }

    return acc;
}
