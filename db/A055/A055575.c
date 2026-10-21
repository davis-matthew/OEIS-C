/*
 * A055575  Sum of digits of n^4 is equal to n.
 * https://oeis.org/A055575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the n‑th integer k such that sum of digits of k^4 is
 *            equal to k
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t digit_sum_uint128(unsigned __int128 x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    while (1) {
        /* compute m^4 as 128 bit integer to avoid overflow */
        unsigned __int128 power = (unsigned __int128)m * m * m * m;
        seq_t sum = digit_sum_uint128(power);

        if (sum == (seq_t)m) {
            ++count;
            if (count == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
