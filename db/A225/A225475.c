/*
 * A225475  Triangle read by rows, k!*s_2(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 154,
 *            u128 n <= 412, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A225475 – Triangle read by rows, k!*s_2(n, k) where s_2(n, k)
 *  are the Stirling–Frobenius cycle numbers of order 2.
 *
 *  For n ≥ 0 let r be the row index such that
 *      r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *  and let k = n – r(r+1)/2   (0 ≤ k ≤ r).
 *  Then
 *      a(n) = k! * s_2(r, k)
 *  where s_2(r, k) is the coefficient of x^k in the polynomial
 *
 *      ∏_{i=0}^{r-1} (x + (2i+1))
 *
 *  This program computes the required coefficient by simple dynamic
 *  programming.  The algorithm runs in O(r^2) time, with r ≤ 141 for
 *  n ≤ 10000, well within the 2‑second requirement.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the flattened triangle
     * by solving r(r+1)/2 ≤ n < (r+1)(r+2)/2.
     * The following linear scan is sufficiently fast for the
     * problem bounds (r ≈ √(2n)).  128‑bit arithmetic prevents overflow. */
    idx_t r = 0;
    while (1) {
        idx_t next = r + 1;
        unsigned __int128 sum = (unsigned __int128)next * (next + 1) / 2;
        if (sum > n)
            break;
        r = next;
    }
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t prevSum = (idx_t)prev;          /* r(r+1)/2  */
    idx_t k = n - prevSum;                /* 0 ≤ k ≤ r */

    /* Small dynamic arrays to hold coefficients of the polynomial
     * ∏_{i=0}^{r-1} (x + (2i+1)).  We use two alternating buffers
     * to avoid repeated allocations. */
    size_t len = (size_t)r;
    seq_t *a_buf = (seq_t*)malloc((len + 1) * sizeof(seq_t));
    seq_t *b_buf = (seq_t*)malloc((len + 1) * sizeof(seq_t));
    if (!a_buf || !b_buf)
        return 0;        /* In a real program we would handle this error. */

    a_buf[0] = 1;
    size_t current_len = 0;   /* Number of meaningful entries in a_buf (before step i) */

    for (size_t i = 1; i <= len; ++i) {
        seq_t step = (seq_t)(2 * i - 1);   /* 2i-1 */
        b_buf[0] = step * a_buf[0];
        for (size_t k_idx = 1; k_idx <= i; ++k_idx) {
            seq_t prev_minus = a_buf[k_idx - 1];
            seq_t prev_val   = (k_idx <= current_len) ? a_buf[k_idx] : (seq_t)0;
            b_buf[k_idx] = prev_minus + step * prev_val;
        }
        /* swap buffers for next iteration */
        seq_t *tmp = a_buf;
        a_buf = b_buf;
        b_buf = tmp;
        current_len = i;
    }

    /* a_buf now holds s_2(r, k) for k=0..r.  Multiply by k! to obtain T(r,k). */
    seq_t fact = 1;
    for (size_t f = 1; f <= k; ++f)
        fact *= (seq_t)f;

    seq_t result = a_buf[k] * fact;

    free(a_buf);
    free(b_buf);
    return result;
}
