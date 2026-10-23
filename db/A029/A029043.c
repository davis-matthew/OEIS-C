/*
 * A029043  Expansion of 1/((1-x)(1-x^3)(1-x^5)(1-x^11)).
 * https://oeis.org/A029043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+30*n^2+371*n+1188-330*floor((n+2)/3))/990)
 * Range:     u8 none, u16 n <= 30, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s   = (seq_t)n;
    seq_t t1    = n_s * n_s * n_s;               /* n^3 */
    seq_t t2    = (seq_t)30 * n_s * n_s;         /* 30*n^2 */
    seq_t t3    = (seq_t)371 * n_s;              /* 371*n */
    seq_t t4    = (seq_t)1188;                   /* constant term */
    idx_t f     = (n + 2) / 3;                   /* floor((n+2)/3) */
    seq_t t5    = (seq_t)330 * (seq_t)f;         /* 330*floor((n+2)/3) */
    seq_t numer = t1 + t2 + t3 + t4 - t5;
    return numer / (seq_t)990;
}
