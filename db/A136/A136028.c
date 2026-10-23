/*
 * A136028  Expansion of (phi(q) * phi(q^2))^3 in powers of q where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A136028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 107, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (phi(q)*phi(q^2))^3.
   a(n) is the coefficient of q^n, where
   φ(q) = 1 + 2∑_{k≥1} q^{k^2}.                               */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Integer square‑root (floor).  Uses only integer arithmetic. */
static inline idx_t isqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Pre‑allocate coefficient arrays for φ(q)^3 and φ(q^2)^3. */
    seq_t *phi3   = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *phi2_3 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!phi3 || !phi2_3) return 0;   /* allocation failure protects */

    /* Build φ(q)^3 coefficients. */
    {
        idx_t m1 = isqrt(n);
        for (idx_t i = 0; i <= m1; ++i) {
            idx_t sq_i = i * i;
            seq_t wi = (i == 0 ? (seq_t)1 : (seq_t)2);
            for (idx_t j = 0; j <= m1; ++j) {
                idx_t sq_j = j * j;
                seq_t wj = (j == 0 ? (seq_t)1 : (seq_t)2);
                idx_t sum2 = sq_i + sq_j;
                if (sum2 > n) break;
                seq_t wij = wi * wj;
                for (idx_t k = 0; k <= m1; ++k) {
                    idx_t sum = sum2 + k * k;
                    if (sum > n) break;
                    seq_t wk = (k == 0 ? (seq_t)1 : (seq_t)2);
                    phi3[sum] += wij * wk;
                }
            }
        }
    }

    /* Build φ(q^2)^3 coefficients.  Squares are 2*k^2. */
    {
        idx_t m2 = isqrt(n / 2);
        for (idx_t i = 0; i <= m2; ++i) {
            idx_t sq_i = 2 * i * i;
            seq_t wi = (i == 0 ? (seq_t)1 : (seq_t)2);
            for (idx_t j = 0; j <= m2; ++j) {
                idx_t sq_j = 2 * j * j;
                seq_t wj = (j == 0 ? (seq_t)1 : (seq_t)2);
                idx_t sum2 = sq_i + sq_j;
                if (sum2 > n) break;
                seq_t wij = wi * wj;
                for (idx_t k = 0; k <= m2; ++k) {
                    idx_t sum = sum2 + 2 * k * k;
                    if (sum > n) break;
                    seq_t wk = (k == 0 ? (seq_t)1 : (seq_t)2);
                    phi2_3[sum] += wij * wk;
                }
            }
        }
    }

    /* Convolution: a(n) = ∑_{s=0}^{n} φ3[s] * φ2_3[n-s]. */
    seq_t result = (seq_t)0;
    for (idx_t s = 0; s <= n; ++s)
        result += phi3[s] * phi2_3[n - s];

    /* Clean up. */
    free(phi3);
    free(phi2_3);

    return result;
}
