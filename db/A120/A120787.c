/*
 * A120787  Denominators of partial sums of Catalan numbers scaled by powers
 *          of 1/20.
 * https://oeis.org/A120787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of the rational number r(n) = Σ_{k=0}^n
 *            Catalan(k) / 20^k. The denominator is 20^n / gcd( N(n), 20^n ),
 *            where N(n) = Σ_{k=0}^n Catalan(k)·20^{n-k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Return the exponent of the prime p in Catalan(k). */
static idx_t catalan_vp(idx_t k, unsigned int p)
{
    /* v_p(Catalan(k)) = v_p(binomial(2k,k)) - v_p(k+1)           */
    idx_t vp_binom = 0;
    idx_t pk = p;
    while (pk <= 2ULL * k) {
        vp_binom += (2ULL * k) / pk - 2ULL * (k / pk);
        /* avoid overflow when multiplying pk by p */
        if (pk > UINT64_MAX / p) break;
        pk *= p;
    }

    idx_t vp_k1 = 0;
    idx_t t = k + 1ULL;
    while (t % p == 0) {
        ++vp_k1;
        t /= p;
    }

    return vp_binom > vp_k1 ? vp_binom - vp_k1 : 0;
}

seq_t a(idx_t n)
{
    /* Precompute the minimal 2- and 5-exponents appearing in N(n). */
    idx_t min_v2 = 2ULL * n + 4;   /* large enough initial value   */
    idx_t min_v5 = n + 4;          /* large enough initial value   */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t v2 = catalan_vp(i, 2);
        idx_t v5 = catalan_vp(i, 5);

        idx_t cand2 = v2 + 2ULL * (n - i);
        if (cand2 < min_v2) min_v2 = cand2;

        idx_t cand5 = v5 + (n - i);
        if (cand5 < min_v5) min_v5 = cand5;
    }

    /* Exponents of 2 and 5 that survive in the denominator. */
    idx_t exp2 = 2ULL * n - min_v2;
    idx_t exp5 = n - min_v5;

    /* Compute 2^exp2 and 5^exp5 and multiply. */
    seq_t denom = pow2(exp2) * power5(exp5);
    return denom;
}
