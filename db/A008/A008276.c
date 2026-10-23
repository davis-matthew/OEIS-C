/*
 * A008276  Triangle of Stirling numbers of first kind, s(n, n-k+1), n >= 1, 1
 *          <= k <= n. Also triangle T(n,k) giving coefficients in expansion
 *          of n!*binomial(x,n)/x in powers of x.
 * https://oeis.org/A008276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, nice
 * Formula:   a(n) is the (n‑th) element of the triangle of signed Stirling
 *            numbers of the first kind s(n, n‑k+1). If n is at position r‑th
 *            row (row counting from 1) and column c (within that row), then
 *            a(n) = s(r, r‑c+1) where s(0,0)=1, s(n,0)=0 (n>0) and s(n,k) =
 *            s(n-1,k-1) - (n-1)*s(n-1,k). The triangle is...
 * Range:     i8 n <= 18, i16 n <= 41, i32 n <= 99, i64 n <= 227,
 *            i128 n <= 622, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for NULL (if needed) */

#define MAX_R 8000    /* safe upper bound for r for typical n ≤ 10⁴ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* out of range, not required but safe */

    /* Find the row r such that r(r+1)/2 ≥ n > (r-1)r/2. */
    idx_t r = 1;
    idx_t sum = 1;         /* cumulative count of elements up to row r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Position c within the row (1‑based). */
    idx_t c = n - (sum - r);          /* n - elements before current row   */
    idx_t k = r - c + 1;              /* column index for s(n,k)            */

    /* If r exceeds the static buffer, we fall back to dynamic allocation.
       For the expected range (n ≤ 10⁴) this never happens. */
    if (r > MAX_R) {
        seq_t *prev = (seq_t*)calloc((size_t)r + 1, sizeof(seq_t));
        seq_t *cur  = (seq_t*)calloc((size_t)r + 1, sizeof(seq_t));
        if (!prev || !cur) return 0;         /* allocation failure */

        prev[0] = 1;
        for (idx_t i = 1; i <= r; ++i) prev[i] = 0;

        for (idx_t i = 1; i <= r; ++i) {
            cur[0] = 0;
            for (idx_t j = 1; j <= i; ++j) {
                cur[j] = prev[j-1] - (seq_t)(i-1) * prev[j];
            }
            for (idx_t j = 0; j <= i; ++j) prev[j] = cur[j];
        }
        seq_t val = prev[k];
        free(cur);
        free(prev);
        return val;
    }

    /* Use small static buffers for speed and portability. */
    static seq_t prev[MAX_R + 1];
    static seq_t cur[MAX_R + 1];

    prev[0] = 1;
    for (idx_t i = 1; i <= r; ++i) prev[i] = 0;  /* clear unused slots */

    for (idx_t i = 1; i <= r; ++i) {
        cur[0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            cur[j] = prev[j-1] - (seq_t)(i-1) * prev[j];
        }
        /* copy current row into prev for next iteration */
        for (idx_t j = 0; j <= i; ++j) prev[j] = cur[j];
    }

    return prev[k];
}
