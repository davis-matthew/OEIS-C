/*
 * A004162  Sum of digits of pentagonal numbers.
 * https://oeis.org/A004162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of n-th pentagonal number P(n) = n(3n-1)/2
 * Range:     u8 n <= 13, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t pent;
    
    if (n & 1) {
        pent = sn * ((3 * sn - 1) / 2);
    } else {
        pent = (sn / 2) * (3 * sn - 1);
    }
    
    seq_t digit_sum = 0;
    while (pent > 0) {
        digit_sum += pent % 10;
        pent /= 10;
    }
    
    return digit_sum;
}
