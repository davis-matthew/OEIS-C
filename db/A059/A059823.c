/*
 * A059823  Expansion of series related to Liouville's Last Theorem: g.f.
 *          sum_{t>0} (-1)^(t+1) *x^(t*(t+1)/2) / ( (1-x^t)^6
 *          *product_{i=1..t} (1-x^i) ).
 * https://oeis.org/A059823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum_{t>0} (-1)^(t+1) * coeff of x^n in
 *   x^{t(t+1)/2} / ( (1-x^t)^6 * prod_{i=1..t} (1-x^i) )
 *
 * Implementation: Iterate over all t with t(t+1)/2 ≤ n.
 * For each t, compute partition numbers p_t(m) for m ≤ n - t(t+1)/2
 * (parts ≤ t). The factor (1-x^t)^-6 contributes C(k+5,5) at x^{k*t}.
 * Convolve p_t with these binomial coefficients, add with sign
 * (-1)^(t+1), and accumulate the result for a(n).
 *
 * Complexity: O(n * sqrt(n)) time and O(n * sqrt(n)) space,
 * well below the 2‑second limit for n up to ~10 000.
 *
 * The code respects all type constraints for seq_t and idx_t.
 */
 
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;

    /* Find tmax with t(t+1)/2 ≤ n */
    idx_t tmax = 0;
    while ((tmax + 1ULL) * (tmax + 2ULL) / 2ULL <= n)
        ++tmax;

    size_t Np1 = (size_t)n + 1U;          /* number of coefficients 0..n */

    /* Allocate partition table P[t][m] for 0 ≤ t ≤ tmax, 0 ≤ m ≤ n */
    seq_t *P = alloc_seq_array((tmax + 1U) * Np1);
    for (size_t i = 0; i < Np1; ++i)
        P[i] = (seq_t)0;                  /* P[0][*] initialized to 0 */
    P[0] = (seq_t)1;                      /* P[0][0] = 1 */

    /* Build partition numbers with restricted parts */
    for (size_t t = 1; t <= (size_t)tmax; ++t) {
        size_t base = t * Np1;
        size_t prev = (t - 1U) * Np1;
        for (size_t m = 0; m < Np1; ++m) {
            seq_t val = P[prev + m];
            if (m >= t)
                val += P[base + (m - t)];
            P[base + m] = val;
        }
    }

    /* Arrays for positive and negative contributions */
    seq_t *plus  = alloc_seq_array(Np1);
    seq_t *minus = alloc_seq_array(Np1);
    for (size_t i = 0; i < Np1; ++i) {
        plus[i]  = (seq_t)0;
        minus[i] = (seq_t)0;
    }

    /* Main loop over t */
    for (size_t t = 1; t <= (size_t)tmax; ++t) {
        size_t offset = t * (t + 1) / 2;      /* x‑shift */
        size_t max_m  = (size_t)n - offset;   /* remaining exponent */

        size_t maxk = max_m / t;              /* maximum k for (1-x^t)^-6 */
        seq_t *w = alloc_seq_array(maxk + 1U);
        w[0] = (seq_t)1;
        for (size_t k = 1; k <= maxk; ++k) {
            seq_t prev = w[k - 1];
            prev = prev * (seq_t)(k + 5U);
            prev = prev / (seq_t)k;
            w[k] = prev;
        }

        seq_t *P_t = &P[t * Np1];

        /* Convolution of P_t with w, scattering into plus/minus */
        for (size_t k = 0; k <= maxk; ++k) {
            seq_t wk = w[k];
            size_t mult = k * t;               /* shift due to (1-x^t)^-6 */
            for (size_t m = 0; m + mult <= max_m; ++m) {
                size_t idx = offset + m + mult;           /* coefficient index */
                if (t & 1U)   /* t odd → positive sign */
                    plus[idx]  += wk * P_t[m];
                else          /* t even → negative sign */
                    minus[idx] += wk * P_t[m];
            }
        }

        free(w);
    }

    seq_t result = plus[(size_t)n];
    result -= minus[(size_t)n];

    free(P);
    free(plus);
    free(minus);

    return result;
}
