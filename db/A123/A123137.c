/*
 * A123137  a(n) = a(n-1)^2 + Sum of the digits of a(n-1), with a(1)=1.
 * https://oeis.org/A123137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   a(n) = a(n-1)^2 + digit_sum(a(n-1))  for n > 1   (OEIS offset 1) */

/* Requires the digit_sum helper defined in utils/digit_sum.h. */
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                   /* undefined, return 0 for safety */

    /* Base case a(1) = 1 */
    seq_t val = (seq_t)1;
    if (n == 1)
        return val;

    /* Iterate from 2 up to n computing the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = digit_sum(val);        /* sum of digits of a(i-1) */
        val = (val * val) + sum;           /* a(i) = a(i-1)^2 + sum */
    }
    return val;
}
