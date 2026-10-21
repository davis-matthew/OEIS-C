/*
 * A089246  Triangular array read by rows: a(n, k) is the number of ordered
 *          m-tuples of positive integers (x_1, ..., x_m) such that max x_i =
 *          n+1-m and there are k ones (0 <= k <= n).
 * https://oeis.org/A089246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   pow_seq_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th element of the triangular array
 * T(r,k).  Here r is the row index (starting at 0) and k the
 * column index (0 ≤ k ≤ r).  The mapping from a linear index n
 * to (r,k) uses triangular numbers:
 *   r  = ⌊(√(8n+1) – 1)/2⌋
 *   k  = n – r(r+1)/2
 *
 * The array element is given by
 *   T(r,k) = 0^(r-k) + Σ_{m=k}^{r-1} C(m,k)·[(r-m)^(m-k) – (r-1-m)^(m-k)].
 *
 * The implementation follows this formula exactly, using only
 * integer arithmetic on the user‑provided types seq_t (the value
 * type) and idx_t (the index type).  All intermediate combinatorial
 * values are kept in seq_t, while row/column indices are computed
 * in idx_t.
 */

#include <stdbool.h>
#include "../utils/pow_seq_small.h"

/* Integer square root with floor: floor(sqrt(n)) for idx_t n. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)   low = mid + 1;
        else          high = mid - 1;
    }
    return high;
}

/* ---- sequence value computation ---- */
seq_t a(idx_t n)
{
    /* Determine (r,k) from the linear index n. */
    idx_t t   = 8 * n + 1;
    idx_t sq  = floor_isqrt_u64(t);          /* floor(√(8n+1))           */
    idx_t r   = (sq - 1) / 2;                /* r satisfies r(r+1)/2 ≤ n */
    idx_t row_start = r * (r + 1) / 2;       /* first index of row r     */
    idx_t k   = n - row_start;               /* column in row r          */

    /* 0^(r-k) term: 1 if k == r (i.e. r-k == 0), else 0. */
    seq_t result = (k == r) ? 1 : 0;

    /* If r > k, sum the remaining terms. */
    if (r > k) {
        seq_t binom = 1;                     /* C(k,k) */
        for (idx_t m = k; m <= r - 1; ++m) {
            idx_t exp = m - k;
            seq_t pow1 = pow_seq_small((seq_t)(r - m), exp);
            seq_t pow2 = pow_seq_small((seq_t)(r - 1 - m), exp);
            seq_t diff = pow1 - pow2;
            result += binom * diff;

            /* Update binomial: C(m+1,k) = C(m,k) * (m+1)/(m+1-k) */
            seq_t numer = binom * (seq_t)(m + 1);
            seq_t denom = (seq_t)(m + 1 - k);   /* exact division */
            binom = numer / denom;
        }
    }

    return result;
}
