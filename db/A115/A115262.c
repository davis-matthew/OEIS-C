/*
 * A115262  Correlation triangle for n+1.
 * https://oeis.org/A115262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A115262 – Correlation triangle for n+1
 *
 *  The triangle T(m,k) counts the number of squares in a rectangle of
 *  dimensions (m+1) × (m+1-k).  It satisfies
 *
 *      T(m,k) = Σ_{j=0}^{min(k,m-k)} (k-j+1)(m-k-j+1)
 *
 *  The flattened sequence a(n) is obtained by reading the triangle
 *  row‑by‑row (row 0: 1; row 1: 2,2; row 2: 3,5,3; …).
 *
 *  This implementation converts the flat index n into a row r and a
 *  column c (0 ≤ c ≤ r) by solving r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 *  then evaluates the closed‑form expression for T(r,c) using integer
 *  arithmetic only.
 *
 *  Complexity: O(log n) per call (binary search for r), otherwise
 *  constant time.  All arithmetic fits in seq_t for the problem
 *  constraints.
 */

#include <stdint.h>

/* ----- helpers ----------------------------------------------------- */

static inline idx_t row_from_index(idx_t n)
{
    /* Find the greatest r such that r*(r+1)/2 <= n.  Binary search
     * on the interval [0, 2*n+1], which is certainly large enough. */
    idx_t low = 0;
    idx_t high = 2 * n + 1;             /* high > sqrt(2*n) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* upper mid */
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Evaluate T(m,k) using the simplified closed form. */
static inline seq_t triangle_value(idx_t m, idx_t k)
{
    /* t = min(k, m-k) */
    idx_t t;
    if (k < m - k)
        t = k;
    else
        t = m - k;

    /* Values used in the expression. */
    seq_t A = (seq_t)(k + 1);          /* k+1 */
    seq_t B = (seq_t)(m - k + 1);      /* m-k+1 */
    seq_t t1 = (seq_t)(t + 1);         /* t+1 */

    /* term1 = (t+1) * A * B */
    seq_t term1 = t1 * A * B;

    /* term2 = (A+B) * t*(t+1)/2 */
    seq_t t_mul = (seq_t)(t * (t + 1) / 2);
    seq_t term2 = (A + B) * t_mul;

    /* term3 = t*(t+1)*(2t+1)/6 */
    seq_t term3 = (seq_t)(t * (t + 1) * (2 * t + 1) / 6);

    return term1 - term2 + term3;
}

/* ----- main routine ------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Convert flat index to row r and column c in the triangle. */
    idx_t r = row_from_index(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    /* Return the triangle entry. */
    return triangle_value(r, c);
}
