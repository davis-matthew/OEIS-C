/*
 * A109120  a(n) = 10*(n+1)^3*(n+2)*(5*n+7)^2.
 * https://oeis.org/A109120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*(n+1)^3*(n+2)*(5*n+7)^2
 * Range:     u8 none, u16 n <= 1, u32 n <= 14, u64 n <= 646, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k  = (seq_t)n;
    seq_t n1 = k + 1;                /* (n+1) */
    seq_t n1_sq = n1 * n1;           /* (n+1)^2 */
    seq_t n1_cu = n1_sq * n1;        /* (n+1)^3 */
    seq_t n2 = k + 2;                /* (n+2) */
    seq_t fp = 5 * k + 7;            /* (5*n+7) */
    seq_t fp_sq = fp * fp;           /* (5*n+7)^2 */
    seq_t result = 10 * n1_cu;       /* 10*(n+1)^3 */
    result *= n2;                   /* * (n+2) */
    result *= fp_sq;                /* * (5*n+7)^2 */
    return result;
}
