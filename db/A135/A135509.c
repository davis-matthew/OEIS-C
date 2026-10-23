/*
 * A135509  Nonnegative integers c such that there are nonnegative integers a
 *          and b that satisfy a^(1/2) + b^(1/2) = c^(1/2) and a^2 + b = c.
 * https://oeis.org/A135509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n^3 + n)/2)^2 (OEIS A135509)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t t  = nn * nn * nn;      /* n^3  */
    t = t + nn;                   /* n^3 + n */
    t = t / 2;                    /* (n^3 + n)/2 */
    return t * t;                 /* square of that */
}
