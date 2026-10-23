/*
 * A014628  Number of segments (and sides) created by diagonals of an n-gon in
 *          general position.
 * https://oeis.org/A014628
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2-5*n+12)/12 (for n >= 3)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* behaviour below offset is irrelevant */

    seq_t sn = (seq_t)n;                /* cast idx_t to seq_t safely */

    seq_t t1    = sn * (sn - 1);                                     /* n*(n-1) */
    seq_t inner = sn * sn - (seq_t)5 * sn + (seq_t)12;              /* n^2-5*n+12 */
    seq_t prod  = t1 * inner;                                       /* n*(n-1)*(n^2-5*n+12) */

    return prod / (seq_t)12;                                       /* divide by 12 exactly */
}
