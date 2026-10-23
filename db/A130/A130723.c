/*
 * A130723  Least common multiple of 3 and n^2+n+1.
 * https://oeis.org/A130723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(3, n^2+n+1)
 * Range:     u8 n <= 8, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t m  = tn * tn + tn + (seq_t)1;
    if (m % (seq_t)3 == (seq_t)0)
        return m;
    else
        return m * (seq_t)3;
}
