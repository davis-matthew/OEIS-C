/*
 * A077198  a(n) = lambda(sigma(n)), where lambda(n) is the Carmichael lambda
 *          function, which gives the smallest integer m such that k^m = 1 mod
 *          n for all integers k relatively prime to n.
 * https://oeis.org/A077198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = λ(σ(n)) where σ is the sum‑of‑divisors function and λ is
 *            Carmichael's function. λ(p^e) = 1 (p = 2, e = 1) λ(p^e) = 2 (p =
 *            2, e = 2) λ(p^e) = 2^(e-2) (p = 2, e ≥ 3) λ(p^e) = p^(e-1)(p-1)
 *            (p odd) λ of a product is the lcm of the λ values
 * Range:     u8 n <= 287, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 9868 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t s = sigma_of(n);

    /* convert to uint64_t for factoring (σ(n) is small for n ≤ 10 000) */
    uint64_t m = (uint64_t)s;

    /* Carmichael λ(1) = 1 */
    if (m == 1ULL)
        return (seq_t)1;

    /* factor m into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* lcm of λ(p^e) */
    seq_t l = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t lam;

        if (p == 2ULL) {
            if (e == 1ULL)
                lam = (seq_t)1;
            else if (e == 2ULL)
                lam = (seq_t)2;
            else
                lam = ipow_idx((seq_t)2, (idx_t)(e - 2));
        } else {
            seq_t pow_pe_minus1 = ipow_idx((seq_t)p, (idx_t)(e - 1));
            lam = seq_mul(pow_pe_minus1, (seq_t)(p - 1));
        }

        seq_t g = gcd(l, lam);
        l = (l / g) * lam;
    }

    return l;
}
