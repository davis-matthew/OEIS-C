/*
 * A159830  Exponential Riordan array [exp(exp(x)-1-2x),x].
 * https://oeis.org/A159830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the flattened row‑major traversal of the triangle *
 *            defined by the exponential Riordan array [exp(exp(x)-1-2x),x]. *
 *            The triangle entries satisfy * * T(n,k) = C(n,k) * d_{n-k} (0 ≤
 *            k ≤ n) * * where the first column d_m is given by * * d_0 = 1 *
 *            d_{m+1} = Σ_{k=0}^{m} C(m,k) * d_k – 2*d_m *...
 * Range:     i8 n <= 28, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Multiplicative computation of binomial coefficient.
 * n and k are non‑negative and n ≥ k.
 * Uses a 128‑bit intermediate to avoid overflow of seq_t
 * as long as the final result fits in seq_t.
 */
static inline seq_t small_binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that the first element of row r has
     * index (r*(r+1))/2, and n lies in that row. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Number of elements preceding this row. */
    idx_t prev_offset = r * (r + 1) / 2;
    idx_t c = n - prev_offset;   /* column within the row */

    /* Build first column d[0 … r] by recurrence. */
    seq_t *d = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!d)
        return (seq_t)0;          /* allocation failure – return 0 */

    d[0] = (seq_t)1;
    for (idx_t m = 0; m < r; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k) {
            sum += small_binom((idx_t)m, (idx_t)k) * d[k];
        }
        d[m + 1] = sum - (seq_t)2 * d[m];
    }

    /* Compute the required triangle entry. */
    seq_t bin = small_binom((idx_t)r, (idx_t)c);
    seq_t result = bin * d[r - c];

    free(d);
    return result;
}
