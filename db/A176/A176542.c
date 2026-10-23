/*
 * A176542  Numbers k such that there are only a finite nonzero number of sets
 *          of k consecutive triangular numbers that sum to a square.
 * https://oeis.org/A176542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 24*n + 16)/2 for n even; * a(n) = (9*n^2 + 30*n
 *            + 25)/2 for n odd
 * Range:     u8 n <= 3, u16 n <= 118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based. For n==0 (unlikely), return 0. */
    if (n == 0) return 0;

    seq_t m = (seq_t)n;           /* work in seq_t to avoid overflow            */
    seq_t term;
    /* compute numerator according to parity of n */
    if ((n & 1) == 0) {           /* n even */
        term = (9 * m * m) + (24 * m) + 16;
    } else {                      /* n odd */
        term = (9 * m * m) + (30 * m) + 25;
    }
    return term / 2;              /* numerator is always even, truncation OK   */
}
