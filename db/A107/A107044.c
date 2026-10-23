/*
 * A107044  A symmetric factorial triangle, read by rows: T(n,k) =
 *          min(k,n-k)!; n>=2, 1<=k<=n-1.
 * https://oeis.org/A107044
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = min(k, r-k)! where n ≥ 2, k is the 1‑based position of
 *            the n‑th term inside its row r (rows start at n = 2). The row r
 *            is the smallest integer with sum_{m=2}^{r} (m-1) ≥ n-1. Then k =
 *            (n-1) - sum_{m=2}^{r-1} (m-1)
 * Range:     u8 n <= 61, u16 n <= 145, u32 n <= 313, u64 n <= 841,
 *            u128 n <= 2381, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed by seq_t implementation */

seq_t a(idx_t n)
{
    /* for safety: ignore indices below offset (n < 2) */
    if (n < 2)
        return (seq_t)0;

    /* m = n - 1 is the 0‑based index of the term in the flattened
       sequence starting at n = 2. */
    idx_t m = n - 1;

    /* Find the row r such that the cumulative number of terms up to
       row r (inclusive) is at least m.  We also keep t_prev = terms up
       to row r-1. */
    idx_t r = 2;          /* rows start at 2 */
    idx_t cum = 0;        /* terms up to previous row */
    idx_t t_prev = 0;     /* terms up to row r-1 */

    while (true)
    {
        t_prev = cum;                     /* store t(r-1) */
        cum += r - 1;                     /* cum = t(r) */
        if (cum >= m)
            break;
        ++r;
    }

    /* Position within the row (0‑based). */
    idx_t pos0 = m - t_prev - 1;          /* 0 ≤ pos0 < r-1 */
    idx_t k = pos0 + 1;                   /* 1‑based position in the row */

    /* m_factorial = min(k, r - k) */
    idx_t m_fact;
    if (k <= (r - k))
        m_fact = k;
    else
        m_fact = r - k;

    /* Compute factorial of m_fact in type seq_t. */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= m_fact; ++i)
        result *= (seq_t)i;

    return result;
}
