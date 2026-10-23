/*
 * A008727  Molien series for 3-dimensional group [2,n] = *22n.
 * https://oeis.org/A008727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n+9} floor(j/9)
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = (n + 9) / 9;                    /* floor((n+9)/9) */
    seq_t term1 = ((seq_t)(n + 10)) * (seq_t)q;           /* (n+9+1)*q */
    seq_t term2 = ((seq_t)9) * (seq_t)q * (seq_t)(q + 1) / 2; /* 9*q*(q+1)/2 */
    return term1 - term2;
}
