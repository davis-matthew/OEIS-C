/*
 * A178646  a(n) = product of divisors d of n such that d^k is not equal to n
 *          for any k >= 1.
 * https://oeis.org/A178646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors d of n such that d^k ≠ n for any k ≥
 *            1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   factor_u64, gcd_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper to compute gcd of two uint64_t numbers */

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 1)
        return seq_one();

    /* factorize n into primes and exponents */
    uint64_t primes[16], exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* number of divisors τ(n) = ∏ (e_i + 1) */
    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (uint64_t)(exps[i] + 1);

    /* product of all divisors: ∏ p_i^{e_i * τ / 2} */
    seq_t P = seq_one();
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i] * tau;   /* always even */
        exp /= 2;
        P = seq_mul(P, ipow_idx((seq_t)primes[i], (idx_t)exp));
    }

    /* gcd of all exponents, determines the set of perfect‑power roots */
    uint64_t G = exps[0];
    for (size_t i = 1; i < cnt; ++i)
        G = gcd_u64(G, exps[i]);

    /* product of all roots r_k = n^{1/k} for each divisor k | G */
    seq_t R = seq_one();
    for (uint64_t k = 1; k <= G; ++k) {
        if (G % k != 0)        /* skip non–divisors of G */
            continue;
        seq_t root = seq_one();
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t e = exps[i] / k;
            if (e > 0)
                root = seq_mul(root, ipow_idx((seq_t)primes[i], (idx_t)e));
        }
        R = seq_mul(R, root);
    }

    /* a(n) = product of all divisors / product of all roots */
    return P / R;
}
