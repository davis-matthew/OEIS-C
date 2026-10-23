/*
 * A014837  Sum of all the digits of n in every base from 2 to n-1.
 * https://oeis.org/A014837
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{b=2}^{n-1} digit_sum(n, b) where digit_sum(n, b) is
 *            the sum of the digits of n when expressed in base b
 * Range:     u8 n <= 33, u16 n <= 598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3; for n < 3 return 0. */
    if (n < 3)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* Iterate over all bases b from 2 up to n-1. */
    for (idx_t b = 2; b < n; ++b)
    {
        idx_t tmp = n;          /* temporary value for division */
        seq_t base_sum = (seq_t)0;

        /* Compute the digit sum of n in base b. */
        while (tmp > 0)
        {
            base_sum += (seq_t)(tmp % b);
            tmp /= b;
        }

        sum += base_sum;
    }

    return sum;   /* a(n) is the accumulated sum of digit sums. */
}
