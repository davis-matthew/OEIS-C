/*
 * A024193  Integer part of (3rd elementary symmetric function of S(n))/(2nd
 *          elementary symmetric of S(n)), where S(n) = {3,4, ..., n+4}.
 * https://oeis.org/A024193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 1/2 * n * (7 + n) * (46 + 13*n + n^2) / (144 +
 *            41*n + 3*n^2) )
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 1486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Offset starts at 1, but guard against zero */
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;
    seq_t part1 = sn * (sn + 7);              /* n * (7 + n) */
    seq_t part2 = n2 + 13 * sn + 46;         /* n^2 + 13*n + 46 */
    seq_t numerator   = part1 * part2;        /* n * (7+n) * (46+13*n+n^2) */
    seq_t denominator = 2 * (144 + 41 * sn + 3 * n2); /* 2 * (144 + 41*n + 3*n^2) */
    return numerator / denominator;          /* integer division = floor */
}
