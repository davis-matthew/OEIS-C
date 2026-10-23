/*
 * A130297  A130296^2.
 * https://oeis.org/A130297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * The sequence is the flattened lower‑triangular array T where *
 *            T(r,1) = r(r+1)/2 + r – 1 (the left border, OEIS A034856) * and
 *            for j >= 2, T(r,j) = r – j + 1 (decreasing by 1 to 1). * a(n) is
 *            the n-th element read row‑by‑row, starting at a(1)=1. * * Thus
 *            a(n) is computed by locating the row r s...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* locate the row r using the inverse of triangular numbers */
    idx_t r = (idx_t)((sqrtl((long double)1 + 8.0L * (long double)n) - 1.0L) / 2.0L);
    /* round up if needed */
    while (( (seq_t)r * ( (seq_t)r + 1 ) ) / 2 < (seq_t)n)
        ++r;

    /* starting index of this row: triangular(r-1)+1 */
    seq_t tri_r_minus1 = ((seq_t)(r - 1) * (seq_t)r) / 2;
    idx_t start = (idx_t)tri_r_minus1 + 1;
    idx_t pos = n - start;                 /* 0‑based position in the row */

    if (pos == 0) {                        /* first element of the row */
        seq_t tri_r = ((seq_t)r * ( (seq_t)r + 1 )) / 2;
        return tri_r + (seq_t)r - 1;
    } else {                                /* other elements are decreasing */
        return (seq_t)(r - pos);
    }
}
