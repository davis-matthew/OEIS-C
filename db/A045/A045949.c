/*
 * A045949  Number of equilateral triangles formed out of matches that can be
 *          found in a hexagonal chunk of side length n in hexagonal array of
 *          matchsticks.
 * https://oeis.org/A045949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(14*n^2 + 9*n + 2)/4)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* promote index to seq_t */
    seq_t t   = 14 * nn * nn + 9 * nn + 2;   /* 14*n^2 + 9*n + 2 */
    seq_t num = nn * t;                      /* n * t          */
    return num / 4;                          /* floor division */
}
