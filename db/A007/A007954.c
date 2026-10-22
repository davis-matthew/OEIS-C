/*
 * A007954  Product of decimal digits of n.
 * https://oeis.org/A007954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, hear
 * Formula:   a(n) = product of decimal digits of n
 * Range:     u8 n <= 487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    
    seq_t result = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 10);
        result *= digit;
        temp /= 10;
    }
    
    return result;
}
