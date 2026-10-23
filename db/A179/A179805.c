/*
 * A179805  a(0) = 1, a(1) = 3, a(2) = 6 and a(n) = 2*a(n-1) - a(n-2) for n >
 *          3.
 * https://oeis.org/A179805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(n)=9*n-12 for n>1 (equivalently a(n)=a(n-1)+9
 *            for n>2)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    return (seq_t)9 * (seq_t)n - (seq_t)12;
}
