/*
 * A093353  a(n) = (n + (n mod 2))*(n + 1)/2.
 * https://oeis.org/A093353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + (n mod 2))*(n + 1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once */
    seq_t nseq = (seq_t)n;
    seq_t m = nseq + 1;           /* (n + 1) */
    seq_t half = (n & 1) ? (m >> 1) : (nseq >> 1);
    /* Return the product (n + (n mod 2))/2 * (n + 1) */
    return half * m;
}
