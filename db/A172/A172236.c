/*
 * A172236  Array A(n,k) = n*A(n,k-1) + A(n,k-2) read by upward antidiagonals,
 *          starting A(n,0) = 0, A(n,1) = 1.
 * https://oeis.org/A172236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 86, u32 n <= 202, u64 n <= 484,
 *            u128 n <= 1209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is read by upward antidiagonals from the array
   A(n,k) = n*A(n,k-1) + A(n,k-2) (k≥2),
   with A(n,0)=0, A(n,1)=1. */

/* compute A(row, k) for small k by the recurrence */
static inline seq_t A_row_k(idx_t row, idx_t k)
{
    if (k == 0u) return 0;
    if (k == 1u) return 1;

    seq_t prev0 = 0;    /* A(row,0) */
    seq_t prev1 = 1;    /* A(row,1) */
    seq_t cur;

    for (idx_t i = 2; i <= k; ++i) {
        cur = (seq_t)row * prev1 + prev0;
        prev0 = prev1;
        prev1 = cur;
    }
    return prev1; /* A(row,k) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;    /* undefined offset, return 0 */

    /* Find diagonal number d such that d*(d+1)/2 >= n */
    idx_t d = 1u;
    idx_t sum = 1u;          /* d*(d+1)/2 for current d */
    while (sum < n) {
        ++d;
        sum += d;
    }

    /* triangular number of (d-1) */
    idx_t tri_prev = (d - 1u) * d / 2u;

    /* position j inside the diagonal: 0 ≤ j ≤ d-1 */
    idx_t j = n - tri_prev - 1u;

    /* row (n) and column (k) in the array */
    idx_t row = d - j;   /* row index (>=1) */
    idx_t k   = j;       /* column index (>=0) */

    return A_row_k(row, k);
}
