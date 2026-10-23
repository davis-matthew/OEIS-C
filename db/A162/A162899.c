/*
 * A162899  Partial sums of [A052938(n)^2].
 * https://oeis.org/A162899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (60 - 36*(-1)^n + (109 - 9*(-1)^n)*n + 24*n^2 + 2*n^3)/24
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index once */
    seq_t sn = (seq_t)n;
    /* n^2 and n^3 */
    seq_t sq = sn * sn;
    seq_t n3 = sq * sn;

    /* compute terms depending on parity of n */
    seq_t term1 = ((n & 1) == 0) ? (seq_t)24 : (seq_t)96;     /* 60 - 36*(-1)^n */
    seq_t term2 = ((n & 1) == 0) ? (seq_t)100 * sn : (seq_t)118 * sn; /* (109 - 9*(-1)^n)*n */
    seq_t term3 = (seq_t)24 * sq;                               /* 24*n^2 */
    seq_t term4 = (seq_t)2 * n3;                                 /* 2*n^3 */

    /* sum and divide by 24 */
    seq_t num = term1 + term2 + term3 + term4;
    return num / (seq_t)24;
}
