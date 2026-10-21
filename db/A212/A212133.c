/*
 * A212133  Number of (w,x,y,z) with all terms in {1,...,n} and median=mean.
 * https://oeis.org/A212133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (2*n^2 - 3*n + 2)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1290, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 1, a(2) = 8, … */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;                /* n^2 */
    seq_t term = (seq_t)2 * sq;        /* 2*n^2 */
    term = term - (seq_t)3 * sn;       /* 2*n^2 - 3*n */
    term = term + (seq_t)2;            /* 2*n^2 - 3*n + 2 */
    return sn * term;                  /* n * (2*n^2 - 3*n + 2) */
}
