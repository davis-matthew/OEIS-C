/*
 * A136216  Triangle T, read by rows, where T(n,k) = A008544(n-k)*C(n,k) where
 *          A008544 equals the triple factorials in column 0.
 * https://oeis.org/A136216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A008544(r - c) * C(r, c) where the index n (starting from
 *            0) is written as n = r*(r+1)/2 + c , 0 ≤ c ≤ r (i.e. r is the
 *            row number and c the column number of the flattened triangle).
 *            A008544(m) is the "triple factorial" defined for m ≥ 0 by
 *            A008544(0) = 1, A008544(m) = (3*m – 1) * A008544(...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column c */
    idx_t base = r * (r + 1) / 2;
    idx_t c = n - base;          /* 0 ≤ c ≤ r */

    /* Compute A008544(r - c) */
    idx_t m = r - c;
    seq_t val = 1;
    for (idx_t i = 1; i <= m; ++i)
        val *= (seq_t)(3 * i - 1);

    /* Compute binomial coefficient C(r, c) (iteratively) */
    idx_t k = c;
    if (k > r - k)
        k = r - k;               /* use symmetry */
    seq_t comb = 1;
    for (idx_t i = 1; i <= k; ++i)
        comb = comb * (seq_t)(r - k + i) / (seq_t)i;

    /* Result */
    return val * comb;
}
