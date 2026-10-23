/*
 * A134447  A002260 * A128174.
 * https://oeis.org/A134447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((i - j + 2)/2) * floor((i + j)/2) where (i, j) are
 *            the row (i ≥ 1) and column (1 ≤ j ≤ i) indices obtained by
 *            unranking n in the triangular array: n = i(i+1)/2 - (i-j). This
 *            gives the OEIS sequence A134447
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based.  For safety return 0 for n == 0. */
    if (n == 0) return 0;

    /* Find the row i such that i(i+1)/2 ≥ n.  */
    idx_t i = 1;
    while (i * (i + 1) / 2 < n) {
        ++i;
    }

    /* Compute the previous triangular number.  */
    idx_t prev_tri = (i - 1) * i / 2;

    /* Column index j (1‑based).  */
    idx_t j = n - prev_tri;

    /* Compute the two floor halves using seq_t arithmetic.  */
    seq_t part1 = (seq_t)(i - j + 2) / 2;   /* floor((i - j + 2)/2) */
    seq_t part2 = (seq_t)(i + j) / 2;       /* floor((i + j)/2)   */

    return part1 * part2;
}
