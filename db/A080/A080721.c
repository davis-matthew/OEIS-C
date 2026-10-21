/*
 * A080721  Triangle of binomial(n,k)*(binomial(n+k,k)-binomial(n+k-2,k-1)).
 * https://oeis.org/A080721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(n,k) * ( binomial(n+k,k) - binomial(n+k-2,k-1)
 *            ), where (n,k) are obtained from the flattened linear index n: n
 *            = r*(r+1)/2 + k, 0 ≤ k ≤ r. The function returns the k‑th entry
 *            of row r of the triangle. If a lookup table were used the terms
 *            would be exactly the first 24 given in th...
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 128, u64 n <= 423,
 *            u128 n <= 1517, big any n
 * Helpers:   binom_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We are allowed to use header <math.h> for sqrtl only. */
#include <stdint.h>
#include <math.h>
#include "../utils/binom_local.h"

/* Compute r (row) from the linear index n.  The result satisfies
   r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static inline idx_t
row_from_index(idx_t n)
{
    /* Use long double for enough precision. */
    long double t = sqrtl((long double)8 * n + 1);
    idx_t r = (idx_t)((t - 1) / 2);

    /* Correct possible rounding errors. */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    while (r * (r + 1) / 2 > n)
        --r;

    return r;
}

/* Main sequence function. */
seq_t
a(idx_t idx)
{
    idx_t r = row_from_index(idx);                 /* row index */
    idx_t k = idx - r * (r + 1) / 2;                /* column index */

    seq_t bin_r_k = binom_local(r, k);
    seq_t bin_rk_k = binom_local(r + k, k);

    seq_t bin_rk_k_m2 = 0;
    if (k > 0 && r + k >= 2)
        bin_rk_k_m2 = binom_local(r + k - 2, k - 1);

    seq_t diff = bin_rk_k - bin_rk_k_m2;
    seq_t result = bin_r_k * diff;

    return result;
}
