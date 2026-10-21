/*
 * A213657  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the graph G(n) consisting of
 *          an edge ab, n vertices c_1, c_2, ..., c_n, and 2n edges ac_i, bc_i
 *          (i=1..n). (n triangles with a common edge).
 * https://oeis.org/A213657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A213657 – Irregular triangle read by rows
 *
 *  The sequence a(n) is obtained by concatenating all rows of the irregular
 *  triangle T(n,k) (1 ≤ k ≤ n+2) where
 *
 *      T(n,k) = 2 * C(n,k-1) + C(n,k-2) + (k==n ? 1 : 0)
 *
 *  C(n,k) is the ordinary binomial coefficient.  Each row has length
 *  n+2, and the cumulative number of entries up to row r is
 *
 *      S(r) = r*(r+5)/2 .
 *
 *  The function below computes a(n) for any n ≥ 1, using the formulas
 *  above and a small multiplicative binomial routine.
 */

#include <stdint.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------
 *  Small binomial routine – works for all seq_t types that support
 *  multiplication and division.  For integer seq_t types the intermediate
 *  arithmetic may overflow but this happens only when the final value
 *  does not fit into seq_t (and is not checked).
 * ----------------------------------------------------------------------- */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;                /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* -----------------------------------------------------------------------
 *  Main sequence function
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row r such that S(r-1) < n ≤ S(r)   */
    idx_t r = 1;
    while ((r * (r + 5)) / 2 < n)
        ++r;

    /* Length of all rows before r */
    idx_t prev = ((r - 1) * ((r - 1) + 5)) / 2;

    /* Position of the term within its row (1‑based) */
    idx_t k = n - prev;          /* 1 ≤ k ≤ r+2 */

    /* Compute the row entry T(r,k) */
    seq_t t1 = binom(r, k - 1);                 /* C(r,k-1)   */
    seq_t t2 = (k >= 2) ? binom(r, k - 2) : 0; /* C(r,k-2)   */
    seq_t result = 2 * t1 + t2;
    if (k == r)
        result += 1;                            /* +1 for k==r */

    return result;
}
