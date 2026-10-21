/*
 * A227327  Number of non-equivalent ways to choose two points in an
 *          equilateral triangle grid of side n.
 * https://oeis.org/A227327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2*n^3 + 8*n^2 - 8*n)/48 if n even (n^4 + 2*n^3 +
 *            8*n^2 - 2*n-9)/48 if n odd (OEIS A227327)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 353, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;                      /* use seq_t for all arithmetic   */
    /* common terms */
    seq_t n2 = n_t * n_t;                      /* n^2  */
    seq_t n3 = n2 * n_t;                      /* n^3  */
    seq_t n4 = n3 * n_t;                      /* n^4  */

    seq_t numerator;
    if ((n & 1) == 0) {                       /* even n */
        numerator = n4 + 2*n3 + 8*n2 - 8*n_t;
    } else {                                  /* odd n  */
        numerator = n4 + 2*n3 + 8*n2 - 2*n_t - 9;
    }
    /* The division is always exact */
    return numerator / 48;
}
