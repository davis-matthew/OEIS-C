/*
 * A066043  a(1) = 1; for m > 0, a(2m) = 2m, a(2m+1) = 4m+2.
 * https://oeis.org/A066043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(1)=1; for m>0, a(2m)=2m, a(2m+1)=4m+2 (i.e. a(n)=n if n is
 *            even, a(n)=2n if n is odd, with the single exception a(1)=1)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if ((n & 1) == 0)           /* n is even */
        return (seq_t)n;
    else                        /* n is odd, n>1 */
        return (seq_t)n * (seq_t)2;
}
