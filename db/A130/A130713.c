/*
 * A130713  a(0)=a(2)=1, a(1)=2, a(n)=0 for n > 2.
 * https://oeis.org/A130713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n, n*n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 2) return (seq_t)1;
    if (n == 1)            return (seq_t)2;
    return (seq_t)0;
}
