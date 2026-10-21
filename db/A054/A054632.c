/*
 * A054632  Partial sums of A007376.
 * https://oeis.org/A054632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the first n decimal digits of Champernowne's
 *            constant (the concatenation of the natural numbers:
 *            123456789101112... )
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t digits_seen = 0;
    /* iterate over positive integers, collecting their decimal digits */
    for (uint64_t num = 1; digits_seen < n; ++num)
    {
        /* gather digits of num in reverse order */
        int rev[20];
        int pos = 0;
        uint64_t temp = num;
        while (temp > 0) {
            rev[pos++] = temp % 10;
            temp /= 10;
        }
        /* num is always >=1, so pos >=1 */

        /* add digits from most significant to least */
        for (int i = pos - 1; i >= 0; --i)
        {
            sum += (seq_t)rev[i];
            ++digits_seen;
            if (digits_seen == n)
                break;
        }
    }
    return sum;
}
