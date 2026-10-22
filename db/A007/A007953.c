/*
 * A007953  Digital sum (i.e., sum of digits) of n; also called digsum(n).
 * https://oeis.org/A007953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice, easy, look
 * Formula:   sum of digits of n
 * Range:     u8 n <= 670, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t num = (seq_t)n;
    
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    
    return sum;
}
