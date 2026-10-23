/*
 * A155546  Triangle read by rows where T(m,n)=2mn + m + n - 5, 1<=n<=m.
 * https://oeis.org/A155546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = 2*m*k + m + k - 5, where m = ceil( (sqrt(1+8*n) - 1) / 2
 *            ); k = n - m*(m-1)/2; The triangle is read by rows of T(m,k) =
 *            2*m*k + m + k - 5
 * Range:     i8 n <= 35, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t / seq_t definitions provided by the build */

/* Helper: compute the m‑th triangular number (m*(m+1))/2 safely. */
static inline idx_t triangular(idx_t m)
{
    return (idx_t)(((__int128)m * (m + 1)) / 2);
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  Behavior for n == 0 is
       unspecified; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Binary search for the row index m such that T(m-1) < n <= T(m). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = triangular(mid);
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Position k of the element within row m (1‑based). */
    idx_t prev_tri = triangular(m - 1);
    idx_t k = n - prev_tri;           /* 1 <= k <= m */

    /* Compute the value using seq_t arithmetic. */
    seq_t sm = (seq_t)m;
    seq_t sk = (seq_t)k;
    seq_t val = 2 * sm * sk + sm + sk - 5;
    return val;
}
