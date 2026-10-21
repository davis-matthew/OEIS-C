/*
 * A087074  For n > 0, 0 <= k <= n^2, T(n,k) is the number of rotationally and
 *          reflectively distinct n X n arrays that contain the numbers 1
 *          through k once each and n^2-k zeros.
 * https://oeis.org/A087074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   a(n) = T(r, k) where (r, k) is the n-th entry in the triangular
 *            table of T(r, k) with r >= 1 and 0 <= k <= r^2, enumerated
 *            row‐wise (first r = 1, k = 0..1; then r = 2, k = 0..4; etc.)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 27, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   cumulative_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/cumulative_terms.h"

/* Helper: falling factorial [m]_k = m * (m-1) * ... * (m-k+1) */
static seq_t falling_factorial(seq_t m, idx_t k)
{
    if (k == 0) return (seq_t)1;
    /* If k > m we return 0. We assume m fits in idx_t (true for used range). */
    if (k > (idx_t)m) return (seq_t)0;
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        res *= (m - (seq_t)i);
    }
    return res;
}

/* Helper: compute T(r, k) according to parity of r */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t r_sq = (seq_t)r * (seq_t)r;
    seq_t part;
    seq_t f_r_sq = falling_factorial(r_sq, k);
    seq_t f_r    = falling_factorial((seq_t)r, k);
    if ((r & 1) == 0) {                 /* even r */
        seq_t f_0 = falling_factorial((seq_t)0, k);
        part = f_r_sq + (seq_t)2 * f_r + (seq_t)5 * f_0;
    } else {                            /* odd r */
        seq_t f_1 = falling_factorial((seq_t)1, k);
        part = f_r_sq + (seq_t)4 * f_r + (seq_t)3 * f_1;
    }
    return part / (seq_t)8;              /* division is exact */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that cumulative_terms(r-1) < n <= cumulative_terms(r) */
    idx_t low = 1, high = 1;
    while (cumulative_terms(high) < n) high <<= 1;      /* double until high is enough */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cumulative_terms(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    idx_t prev = cumulative_terms(r - 1);               /* terms before this row */
    idx_t offset = n - prev - 1;                        /* zero‑based offset in this row */
    idx_t k = offset;                                   /* 0 <= k <= r^2 */

    return compute_T(r, k);
}
