/*
 * A067016  Start with a(0)=1, a(1)=4, a(2)=3, a(3)=2; for n>=3, a(n+1) =
 *          max_i (a(i)+a(n-i)).
 * https://oeis.org/A067016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=3, a(3)=2; for n>=4, a(n) = 2*n-1 if n is
 *            even, 2*n-2 if n is odd
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)2;

    seq_t x = (seq_t)n;
    if ((n & (idx_t)1) == 0)   /* even */
        return 2 * x - (seq_t)1;
    else                       /* odd */
        return 2 * x - (seq_t)2;
}
