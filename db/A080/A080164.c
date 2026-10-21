/*
 * A080164  Wythoff difference array, D={d(i,j)}, by antidiagonals.
 * https://oeis.org/A080164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(i·φ)·F[2j‑1] + (i‑1)·F[2j‑2] where (i,j) are
 *            obtained from n by the antidiagonal enumeration: let k be the
 *            unique integer with k·(k+1)/2 ≥ n, let prev = (k‑1)·k/2, let pos
 *            = n – prev (1‑based inside diagonal k), then i = pos, j = k –
 *            pos + 1. φ is the golden ratio and F[m] is the m‑t...
 * Range:     u8 n <= 22, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* (n==0) is outside the offset; return 0 */
    if (n == 0)
        return 0;

    /* Find the diagonal k such that T_{k-1} < n ≤ T_k,  where
       T_k = k·(k+1)/2.  Since n can be large, use a simple loop
       which is fast enough for the required n (≈10 000). */
    idx_t k = 1;
    while (k * (k + 1) / 2 < n) {
        ++k;
    }

    /* Position within diagonal k */
    idx_t prev = (k - 1) * k / 2;
    idx_t pos  = n - prev;      /* 1‑based */
    idx_t i    = pos;
    idx_t j    = k - pos + 1;   /* 1‑based column index */

    /* floor(i·φ) – φ ≈ 1.6180339887498948482 */
    static const double phi = 1.61803398874989484820458683436563811240;
    seq_t i_phi = (seq_t)((long long)(i * phi));   /* floor(i·φ) as seq_t */

    /* Compute Fibonacci numbers up to F[2j‑1] and F[2j‑2] */
    seq_t f_prev2 = 0;   /* F[0] */
    seq_t f_prev1 = 1;   /* F[1] */
    seq_t f_curr;
    idx_t idx;
    for (idx = 2; idx <= 2 * j - 1; ++idx) {
        f_curr  = f_prev2 + f_prev1;
        f_prev2 = f_prev1;
        f_prev1 = f_curr;
    }
    seq_t f2j_1 = f_prev1;  /* F[2j‑1] */
    seq_t f2j_2 = f_prev2;  /* F[2j‑2] */

    /* Compute the value d(i,j) */
    seq_t term1 = i_phi * f2j_1;
    seq_t term2 = (seq_t)(i - 1) * f2j_2;
    return term1 + term2;
}
