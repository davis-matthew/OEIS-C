/*
 * A094952  A sequence derived from pentagonal numbers, or a Stirling number
 *          of the first kind matrix.
 * https://oeis.org/A094952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2n+1)*(3n+1)/2 (OEIS A094952)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = n;                       /* convert index to seq_t */
    seq_t term = n_t * ((seq_t)2 * n_t + 1);  /* n * (2n+1) */
    term *= ((seq_t)3 * n_t + 1);              /* * (3n+1) */
    term /= 2;                                /* divide by 2 (always integer) */
    return term;
}
