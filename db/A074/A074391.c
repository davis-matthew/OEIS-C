/*
 * A074391  a(n) is the smallest number such that gcd(a(n), sigma(a(n))) = n.
 * https://oeis.org/A074391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‐based computation of A074391: a(n) = min{ k | k% n == 0, gcd(k, σ(k)) = n } */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Euclidean algorithm for the generic seq_t type (assumes % and /= are defined) */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute σ(k) for a 64‑bit integer k.  The result is returned as seq_t. */
static seq_t sigma_of_seq_t(seq_t k)
{
    if (k == 0) return 0;
    if (k == 1) return 1;

    /* k must fit in 64 bits for the factorisation routine. */
    uint64_t ku64 = (uint64_t)k;
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(ku64, primes, exps, &cnt);

    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pow_p = 1;
        for (uint64_t j = 0; j < exps[i]; ++j) pow_p *= p;          /* p^e   */
        seq_t pow_p1 = pow_p * p;                                   /* p^(e+1) */
        seq_t term = (pow_p1 - (seq_t)1) / (p - (seq_t)1);
        total *= term;
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside OEIS offset */
    if (n == 1) return 1;          /* first term is 1 */

    seq_t n_seq = (seq_t)n;
    seq_t base   = n_seq;           /* we test multiples of n */

    /* Iterate over multiples of n until the gcd condition is satisfied. */
    for (seq_t k = base; ; k += base) {
        /* Avoid overflow of k beyond 64 bits, which factor_u64 cannot handle. */
        if ((uint64_t)k > UINT64_MAX) break;

        seq_t sigma = sigma_of_seq_t(k);
        seq_t g     = gcd_seq_t(k, sigma);
        if (g == n_seq) return k;
    }

    /* Fallback – no value found within 64‑bit range. */
    return 0;
}
