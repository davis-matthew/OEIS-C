/*
 * A168413  a(n) = 9*n - a(n-1) - 5, with a(1)=2.
 * https://oeis.org/A168413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n-5 + (n%2==0?9:0))/2, a(1)=2
 * Range:     u8 n <= 27, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num = sn * 9 - 5;
    if ((n & 1) == 0) {
        num += 9;
    }
    return num / 2;
}
