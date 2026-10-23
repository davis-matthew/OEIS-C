/*
 * A164004  Zero together with row 4 of the array in A163280.
 * https://oeis.org/A164004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 0 if n==0; a(1)=5; otherwise a(n) = n*(n+3)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)5;
    seq_t nn = (seq_t)n;
    return nn * (nn + (seq_t)3);
}
