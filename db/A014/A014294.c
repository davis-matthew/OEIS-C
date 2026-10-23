/*
 * A014294  Inverse of 285th cyclotomic polynomial.
 * https://oeis.org/A014294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the coefficients of the reciprocal of the 285‑th cyclotomic
 * polynomial Φ_285(x).  The sequence is periodic with period 285.
 * For n ≥ 0, a(n) = coefficient of xⁿ in 1 / Φ_285(x) =
 *     ∏_{d|285} (1 - xᵈ)^{-μ(285/d)}.
 * The coefficients are obtained by multiplying the series corresponding
 * to each divisor of 285.  The result repeats every 285 terms, so only
 * the first 285 coefficients are needed.
 */
#include <stddef.h>

#define PERIOD 285

static seq_t cache[PERIOD];
static int cache_ready = 0;

static void compute_cache(void)
{
    const idx_t divisors[] = {1, 3, 5, 15, 19, 57, 95, 285};
    const int mu[]        = {-1,  1,  1,  -1,  1, -1, -1, 1};
    const size_t D = sizeof(divisors) / sizeof(divisors[0]);

    seq_t a[PERIOD] = {0};
    seq_t tmp[PERIOD];
    a[0] = 1;

    for (size_t idx = 0; idx < D; ++idx) {
        idx_t d   = divisors[idx];
        int   mu_k = mu[idx];

        if (mu_k == 1) { /* multiply by 1/(1 - x^d) */
            for (idx_t i = 0; i < PERIOD; ++i) tmp[i] = 0;
            for (idx_t i = 0; i < PERIOD; ++i) {
                seq_t v = a[i];
                if (v == 0) continue;
                for (idx_t j = i; j < PERIOD; j += d) {
                    tmp[j] += v;
                }
            }
            for (idx_t i = 0; i < PERIOD; ++i) a[i] = tmp[i];
        } else { /* mu_k == -1, multiply by (1 - x^d) */
            for (idx_t i = 0; i < PERIOD; ++i) {
                seq_t v = a[i];
                if (i >= d) v -= a[i - d];
                tmp[i] = v;
            }
            for (idx_t i = 0; i < PERIOD; ++i) a[i] = tmp[i];
        }
    }

    for (idx_t i = 0; i < PERIOD; ++i) cache[i] = a[i];
}

seq_t a(idx_t n)
{
    if (!cache_ready) {
        compute_cache();
        cache_ready = 1;
    }
    return cache[n % PERIOD];
}
