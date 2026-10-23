/*
 * A169805  Twice the sum of the digits of n.
 * https://oeis.org/A169805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * digit_sum(n) (the sum of decimal digits of n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp) {
        sum += tmp % 10;
        tmp /= 10;
    }
    return sum << 1;      /* multiply by 2 */
}
