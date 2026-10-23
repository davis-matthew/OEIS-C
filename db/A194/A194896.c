/*
 * A194896  Triangular array (and fractal sequence): row n is the permutation
 *          of (1,2,...,n) obtained from the increasing ordering of fractional
 *          parts {r}, {2r}, ..., {nr}, where r=-sqrt(8).
 * https://oeis.org/A194896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the flattened triangle whose row r is
 *            the permutation of (1,2,…,r) obtained by ordering the indices
 *            decreasingly by the fractional parts {k*sqrt(8)}
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Constant sqrt(8) with sufficient precision for the required range.  */
static const long double SQRT8 = 2.828427124746190097603377448L;

/* Compare two indices by decreasing order of {k*sqrt(8)}.  */
static int cmp_frac_desc(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;

    /* Compute fractional part with integer truncation.  */
    long double fa = a * SQRT8 - (uint64_t)(a * SQRT8);
    long double fb = b * SQRT8 - (uint64_t)(b * SQRT8);

    if (fa > fb) return -1;
    if (fa < fb) return 1;
    /* Tie-breaking by numeric value to keep qsort stable.  */
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Find the row r such that triangular(r-1)+1 <= n <= triangular(r).  */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 < n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;            /* For completeness, though n>=1 in OEIS. */

    /* Determine which row contains the n-th term. */
    idx_t row = find_row(n);
    /* Position inside this row (1-based). */
    idx_t pos = n - (idx_t)((seq_t)(row - 1) * row / 2);

    /* Allocate array of indices 1..row */
    uint64_t *idx = (uint64_t *)malloc((size_t)row * sizeof(uint64_t));
    if (!idx) return 0;              /* Allocation failure. */

    for (idx_t i = 0; i < row; ++i)
        idx[i] = (uint64_t)(i + 1);

    /* Sort indices by decreasing fractional part of k*sqrt(8). */
    qsort(idx, (size_t)row, sizeof(uint64_t), cmp_frac_desc);

    /* Retrieve the element at position 'pos' (1-based). */
    uint64_t result = idx[pos - 1];
    free(idx);
    return (seq_t)result;
}
