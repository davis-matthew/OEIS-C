/*
 * A036697  a(n)=number of Gaussian integers z=a+bi satisfying n-1<|z|<=n,
 *          a>=0, b>=0.
 * https://oeis.org/A036697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of Gaussian integers z=a+bi with n-1 < |z| ≤ n, a≥0,
 *            b≥0
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The distance condition is equivalent to (n-1)² < a²+b² ≤ n².     */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 specifically */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Square of (n-1) and n, computed in seq_t to avoid overflow */
    idx_t n_minus1 = n - (idx_t)1;
    seq_t R1_sq = (seq_t)n_minus1 * (seq_t)n_minus1;   /* (n-1)² */
    seq_t R2_sq = (seq_t)n * (seq_t)n;                 /* n²  */

    seq_t result = (seq_t)0;

    for (idx_t a_val = (idx_t)0; a_val <= n; ++a_val) {
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;            /* a²  */

        /* Number of b values with a² + b² ≤ n² */
        seq_t b_max = floor_sqrt(R2_sq - a_sq);             /* floor sqrt */

        seq_t cnt;
        if (a_sq > R1_sq) {
            /* All b from 0 to b_max inclusive satisfy strict lower bound */
            cnt = b_max + (seq_t)1;
        } else {
            /* Lower bound is active, subtract floor sqrt((n-1)² – a²) */
            seq_t lower_sq = R1_sq - a_sq;                  /* non‑negative */
            seq_t s = floor_sqrt(lower_sq);
            cnt = b_max - s;
        }

        result += cnt;
    }

    return result;
}
