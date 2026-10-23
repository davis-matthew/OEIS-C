/*
 * A019430  Continued fraction for tan(1/7).
 * https://oeis.org/A019430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = (-1+3*(-1)^n-7*(-1+(-1)^n)*n)/2
 * Range:     u8 n <= 36, u16 n <= 9362, u32 n <= 9937, u64 n <= 9937,
 *            u128 n <= 9937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases: a(0)=0, a(1)=6.  For n>1          */
/*   if n even  → a(n)=1                                */
/*   if n odd   → a(n)=7*n-2                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)6;
    if ((n & 1) == 0)   /* even n > 1 */
        return (seq_t)1;
    /* odd n > 1 */
    return ((seq_t)7 * (seq_t)n) - (seq_t)2;
}
