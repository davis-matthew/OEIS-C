/*
 * A061891  a(0) = 1; for n>0, a(n) = a(n-1) if n is already in the sequence,
 *          a(n) = a(n-1) + 3 otherwise.
 * https://oeis.org/A061891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n+1 if n%3==0; 2*n-1 if n%3==1; 2*n if n%3==2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t nn = (seq_t)n;
    if (r == 0)
        return nn * 2 + 1;
    else if (r == 1)
        return nn * 2 - 1;
    else
        return nn * 2;
}
