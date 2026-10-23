/*
 * A152878  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} with maximal number of initial entries of the same
 *          parity equal to k (1 <= k <= ceiling(n/2)).
 * https://oeis.org/A152878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry when the triangle T(n,k) (1 ≤ k ≤ ⌈n/2⌉)
 *            is read row by row. The entries are given by T(2m , k) = 2m · m!
 *            · (2m−k−1)! · binomial(m , k) T(2m+1 , k) = m! · (m+1)! ·
 *            binomial(2m−k+1 , m)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

static inline idx_t row_sum(idx_t r)
{
    /* S(r) = sum_{i=1}^r ceil(i/2) */
    if (r & 1U) {
        idx_t m = r >> 1;
        return (m + 1U) * (m + 1U);
    } else {
        idx_t m = r >> 1;
        return m * (m + 1U);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range – never used */

    /* Find smallest r with row_sum(r) >= n */
    idx_t low  = 1, high = 1;
    while (row_sum(high) < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (row_sum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;
    idx_t prev_total = (row > 0) ? row_sum(row - 1) : 0;
    idx_t col = n - prev_total;      /* 1‑based column */

    /* Compute T(row, col) */
    if ((row & 1U) == 0) {   /* even row: row = 2m */
        idx_t m = row >> 1;
        idx_t k = col;
        seq_t res = factorial(k);
        res *= factorial(row - k - 1);
        res *= binomial(m, k);
        res *= (seq_t)row;           /* row == 2m */
        return res;
    } else {                /* odd row: row = 2m+1 */
        idx_t m = (row - 1) >> 1;
        idx_t k = col;
        seq_t res = factorial(m);
        res *= factorial(m + 1);
        res *= binomial((idx_t)(2 * m - k + 1), m);
        return res;
    }
}
