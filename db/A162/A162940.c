/*
 * A162940  a(n) = binomial(n+1,2)*6^2.
 * https://oeis.org/A162940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n*(n+1)
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n*(n+1) first to keep the intermediate small */
    seq_t t = (seq_t)n * ((seq_t)n + (seq_t)1);
    /* multiply by 18 to obtain 18*n*(n+1) */
    t *= (seq_t)18;
    return t;
}
