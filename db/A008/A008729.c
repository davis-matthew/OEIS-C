/*
 * A008729  Molien series for 3-dimensional group [2, n] = *22n.
 * https://oeis.org/A008729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = Sum_{j=0}^{n+11} floor(j/11)
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = 11*q*(q-1)/2 + q*(r+1)  where q = (n+11)/11, r = (n+11)%11 */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + (seq_t)11;      /* m = n + 11 */
    seq_t q = m / (seq_t)11;             /* q = floor(m/11) */
    seq_t r = m % (seq_t)11;             /* r = m mod 11   */

    /* 11 * q * (q-1) / 2  +  q * (r+1) */
    seq_t term1 = 11 * q * (q - (seq_t)1) / (seq_t)2;
    seq_t term2 = q * (r + (seq_t)1);

    return term1 + term2;
}
