/*
 * A057570  Numbers of the form n*(7n+-1)/2.
 * https://oeis.org/A057570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n-1)+5*(2*n-1)*(-1)^n+5)/16 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert index to seq_t once */
    seq_t base = 14 * nn * (nn - (seq_t)1) + (seq_t)5;   /* 14*n*(n-1)+5 */
    seq_t sub  = 5 * ((seq_t)2 * nn - (seq_t)1);          /* 5*(2*n-1) */
    if (n & 1)   /* n odd: subtract the term */
        return (base - sub) / (seq_t)16;
    else         /* n even: add the term */
        return (base + sub) / (seq_t)16;
}
