/*
 * A216917  Square array read by antidiagonals, T(N,n) = lcm{1<=j<=N,
 *          gcd(j,n)=1 | j} for N >= 0, n >= 1.
 * https://oeis.org/A216917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value read by antidiagonals from the square array T(N,n)
 *            * where T(N,n) = lcm{ j : 1 ≤ j ≤ N and gcd(j,n) = 1 } (N ≥ 0, n
 *            ≥ 1)
 * Range:     u8 n <= 28, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Determine diagonal number d and offset k.
       The d-th diagonal has d terms, and the cumulative number of terms
       up to the (d-1)-th diagonal is d*(d-1)/2. */
    idx_t d = 1;
    idx_t sum = 0;
    while (sum + d < n) {
        sum += d;
        d++;
    }
    /* k is the position (1‑based) within diagonal d. */
    idx_t k = n - sum;          /* k in [1..d] */
    idx_t N = d - k;            /* row index (0‑based) */
    seq_t lcm_val = 1;

    /* Compute lcm of all j ≤ N with gcd(j,k)=1. */
    for (idx_t j = 1; j <= N; ++j) {
        if (gcd((seq_t)j, (seq_t)k) == 1) {
            seq_t g = gcd(lcm_val, (seq_t)j);
            lcm_val = (lcm_val / g) * j;
        }
    }
    return lcm_val;
}
