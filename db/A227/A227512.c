/*
 * A227512  Floor(-1/n + 1/log((2n+1)/(2n-1))).
 * https://oeis.org/A227512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3 - floor(9*n/5) - 1 (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 710, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, safe guard */
    seq_t nn = (seq_t)n;
    /* compute 12 * n^3 */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t term = 12 * n3;         /* 12 * n^3 */
    /* subtract floor(9*n/5) and 1 */
    term -= (9 * nn) / 5;         /* integer division truncates toward zero (floor) */
    term -= 1;
    return term;
}
