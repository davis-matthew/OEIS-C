/*
 * A004160  Sum of digits of the n-th tetrahedral number.
 * https://oeis.org/A004160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A000292(n)), where A000292(n) = n*(n+1)*(n+2)/6
 *            is the n-th tetrahedral number, and A007953 sums digits
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s_n = (seq_t)n;
    seq_t t = s_n * (s_n + 1) * (s_n + 2) / 6;
    
    seq_t digit_sum = 0;
    while (t > 0) {
        digit_sum += t % 10;
        t /= 10;
    }
    
    return digit_sum;
}
