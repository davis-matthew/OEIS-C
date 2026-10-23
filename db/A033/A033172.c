/*
 * A033172  Number of days in n years (n=3 is the first leap year).
 * https://oeis.org/A033172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1461*n+1)/4)
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* According to the OEIS entry, the number of days in n years is
       floor((1461*n+1)/4).  All calculations are performed with seq_t
       to preserve precision for the arbitrary‑precision build, and to
       keep intermediate results within the range of seq_t for fixed
       size builds. */
    seq_t tmp = (seq_t)1461 * (seq_t)n + 1;
    tmp /= 4;
    return tmp;
}
