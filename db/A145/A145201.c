/*
 * A145201  Triangle read by rows: T(n,k) = S(n,k) mod n, where S(n,k) =
 *          Stirling numbers of the first kind.
 * https://oeis.org/A145201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise reading of the triangle T(n,k) =
 *            Stirling‑first‑kind S(n,k) modulo n. For a given linear index n
 *            (starting at 1) we locate the corresponding row r and column c
 *            and compute S(r,c) mod r
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine below works for any idx_t and seq_t that support
   the usual arithmetic operations.  It uses O(r²) operations
   per query, where r ≈ √(2n).  For n ≤ 10 000 the maximum
   row is 141, so the run time is trivial. */

#include <stddef.h>    /* for size_t */
#include <stdint.h>    /* for uint64_t */

static seq_t compute_T(idx_t row, idx_t col)
{
    /* Limit: rows do not exceed 200 for the given constraints. */
    static seq_t prev[201];
    static seq_t curr[201];
    seq_t mod = (seq_t)row;
    /* Base case S(1,1) = 1 */
    prev[1] = 1 % mod;
    if (row == 1)
        return curr[col] = prev[col] % mod;

    for (idx_t m = 2; m <= row; ++m) {
        for (idx_t k = 1; k <= m; ++k) {
            seq_t val = 0;
            if (k <= m - 1) {
                /* (m-1) * S(m-1,k) */
                val = ((seq_t)(m - 1) * prev[k]) % mod;
            }
            if (k > 1) {
                val += prev[k - 1];
                if (val >= mod) val -= mod;
            }
            curr[k] = val % mod;
        }
        /* copy curr into prev for next iteration */
        for (idx_t j = 1; j <= m; ++j) prev[j] = curr[j];
    }
    return curr[col];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but safe */

    /* Determine row r such that r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Column c = n - r(r-1)/2 */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t c = n - prev_tri; /* 1‑based column */

    return compute_T(r, c);
}
