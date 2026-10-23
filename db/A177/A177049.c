/*
 * A177049  Numerator of (3n+1)*(3n+2)/4.
 * https://oeis.org/A177049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, less, easy
 * Formula:   a(n) = numerator of (3n+1)*(3n+2)/4
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (3n+1)*(3n+2) */
    seq_t tn   = (seq_t)n;        /* cast index to seq_t first */
    seq_t t3n  = tn * 3;
    seq_t u    = t3n + 1;
    seq_t v    = t3n + 2;
    seq_t prod = u * v;

    /* determine the gcd with 4 quickly */
    unsigned int nmod = (unsigned int)(n & 3); /* n % 4 */
    seq_t g = ((nmod == 1) || (nmod == 2)) ? (seq_t)4 : (seq_t)2;

    return prod / g;
}
