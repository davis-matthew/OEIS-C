/*
 * A187017  Array where the n-th row is the expansion of (1+2*x+...+n*x^(n-1))
 *          * (n+(n-1)*x+...+x^(n-1)).
 * https://oeis.org/A187017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) : concatenate coefficients of (1+2x+...+n x^(n-1))*(n+(n-1)x+...+x^(n-1)) by row. */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root (floor) using binary search.  */
static idx_t isqrt(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset 1 – n>=1 in normal usage */

    /* Find the row r that contains the n-th term:
       (r-1)^2 < n <= r^2.  */
    idx_t r = isqrt(n);
    if (r * r < n) r++;

    /* Index inside the row (0‑based).  */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t pos = n - prev_sq - 1;

    /* Convolution of sequences P_i = i+1 (0<=i<r) and Q_j = r-j.  */
    idx_t L = (pos > r - 1) ? pos - (r - 1) : 0;
    idx_t U = (pos < r - 1) ? pos : r - 1;

    seq_t result = 0;
    for (idx_t i = L; i <= U; ++i) {
        seq_t a_coef = (seq_t)(i + 1);          /* coefficient from P_i */
        seq_t b_coef = (seq_t)(r - pos + i);    /* coefficient from Q_{pos-i} */
        result += a_coef * b_coef;
    }
    return result;
}
