/*
 * A040997  Absolute value of first digit of n minus sum of other digits of n.
 * https://oeis.org/A040997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = |first digit of n minus sum of the other digits|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Single‑digit numbers return themselves */
    if (n < 10)
        return (seq_t)n;

    /* Find the most significant digit (first digit) */
    idx_t tmp = n;
    int msd = 0;
    while (tmp >= 10) {
        tmp /= 10;
    }
    msd = (int)tmp;

    /* Sum all decimal digits of n */
    seq_t sum = 0;
    tmp = n;
    while (tmp) {
        sum += (seq_t)(tmp % 10);
        tmp /= 10;
    }

    /* Remove the first digit from the total sum */
    sum -= (seq_t)msd;

    /* Compute the absolute difference */
    if ((seq_t)msd >= sum)
        return (seq_t)msd - sum;
    else
        return sum - (seq_t)msd;
}
