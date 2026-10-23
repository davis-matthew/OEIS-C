/*
 * A164016  6 times centered hexagonal numbers: a(n) = 18*n*(n+1) + 6.
 * https://oeis.org/A164016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*n*(n+1) + 6
 * Range:     u8 n <= 3, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t to avoid mixing types before multiplication */
    seq_t n_seq = (seq_t)n;
    seq_t term = n_seq * ((seq_t)(n + 1)); /* n*(n+1) */
    term *= (seq_t)18;                     /* 18*n*(n+1) */
    term += (seq_t)6;                      /* +6 */
    return term;
}
