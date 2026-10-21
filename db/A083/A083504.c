/*
 * A083504  Triangle read by rows: for 1 <= k <= n, T(n, k) is the total
 *          perimeter of all squares contained in a square grid with n rows
 *          and k columns.
 * https://oeis.org/A083504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) where T(n,k) = (2*k^3*n + 6*k^2*n + k^2 +
 *            4*k*n + 2*k - k^4 - 2*k^3) / 3 'row' is the smallest integer
 *            with row*(row+1)/2 >= n 'col' = n - row*(row-1)/2 This
 *            implements the exact sequence A083504
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_of_idx(idx_t n)
{
    /* Find smallest r with r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)      /* defined for n >= 1, but return 0 for safety */
        return 0;

    /* Determine the row and column from the linear index n */
    idx_t row = row_of_idx(n);
    __uint128_t prev_terms = (__uint128_t)row * (row - 1) / 2;
    idx_t col = n - (idx_t)prev_terms;

    /* Use the formula with seq_t arithmetic.  'row' is N, 'col' is K */
    seq_t N = (seq_t)row;
    seq_t K = (seq_t)col;

    seq_t K2 = K * K;          /* k² */
    seq_t K3 = K2 * K;         /* k³ */
    seq_t K4 = K3 * K;         /* k⁴ */

    seq_t term1 = 2 * K3 * N;  /* 2*k³*n */
    seq_t term2 = 6 * K2 * N;  /* 6*k²*n */
    seq_t term3 = K2;          /* k²   */
    seq_t term4 = 4 * K * N;   /* 4*k*n */
    seq_t term5 = 2 * K;       /* 2*k  */
    seq_t term6 = K4;          /* k⁴   */
    seq_t term7 = 2 * K3;      /* 2*k³ */

    seq_t numerator = term1 + term2 + term3 + term4 + term5 - term6 - term7;

    return numerator / 3;
}
