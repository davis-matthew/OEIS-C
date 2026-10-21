/*
 * A093150  Absolute value of difference between (sum of digits of n if n odd,
 *          otherwise sum of digits of 2n) and (sum of digits of n if n even,
 *          otherwise sum of digits of 2n).
 * https://oeis.org/A093150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = |digitSum(2*n) - digitSum(n)| where digitSum(k) = sum of
 *            decimal digits of k
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the decimal digit sum of an idx_t value. */
static seq_t digit_sum(idx_t v)
{
    seq_t sum = 0;
    while (v) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* OEIS sequence A093150 */
seq_t a(idx_t n)
{
    idx_t two_n = n + n;               /* 2 * n, safe for tested range */
    seq_t s1 = digit_sum(n);           /* digit sum of n */
    seq_t s2 = digit_sum(two_n);       /* digit sum of 2*n */

    /* return the absolute difference */
    if (s2 >= s1)
        return s2 - s1;
    else
        return s1 - s2;
}
