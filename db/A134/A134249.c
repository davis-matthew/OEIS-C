/*
 * A134249  Triangle read by rows, taken from the lower triangular matrix (M *
 *          A000012 + A000012 * M) - A000012; where M = lower triangular
 *          matrix with (1,1,1,...) in the main diagonal and the triangular
 *          numbers in the subdiagonal and A000012 = (1; 1,1; 1,1,1; ...).
 * https://oeis.org/A134249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n >= 1, let r be the unique integer such that r(r+1)/2 >= n
 *            > r(r-1)/2. Let j = n - r(r-1)/2. Then a(n) = 1 if j == r a(n) =
 *            1 + T_{r-1} + T_j otherwise, where T_m = m(m+1)/2 (triangular
 *            number)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t triangular(idx_t m)
{
    /* compute m*(m+1)/2 avoiding overflow when possible */
    if (m & 1)
        return ((seq_t)(m + 1) / 2) * (seq_t)m;
    else
        return ((seq_t)m / 2) * (seq_t)(m + 1);
}

seq_t a(idx_t n)
{
    /* handle offset 1 */
    if (n == 0) return 0;          /* value undefined for n=0; not used */

    /* find the row r such that r(r+1)/2 >= n > r(r-1)/2 */
    idx_t r = 1;
    while ( (r * (r + 1)) / 2 < n ) ++r;

    /* previous rows count */
    idx_t prev_rows = (r - 1) * r / 2;
    idx_t j = n - prev_rows;       /* column index in this row (1‑based) */

    if (j == r) return 1;          /* last element of the row */

    seq_t tri_r_minus_1 = triangular(r - 1);  /* T_{r-1} */
    seq_t tri_j        = triangular(j);       /* T_j */
    return 1 + tri_r_minus_1 + tri_j;
}
