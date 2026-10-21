/*
 * A071294  Number of witnesses for strong pseudoprimality of 2n+1; i.e.,
 *          number of bases b, 1 <= b <= 2n, in which 2n+1 is a strong
 *          pseudoprime.
 * https://oeis.org/A071294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (# of bases b, 1 ≤ b ≤ 2n, for which 2n+1 is a strong
 *            pseudoprime). Computed via the known closed form: If 2n+1 is
 *            prime: a(n) = 2n. Else let k = ω(2n+1) be the number of distinct
 *            prime factors, let k1 = min_{p|2n+1} v₂(p−1) (the 2‑adic
 *            valuation of p−1), let q = odd part of (2n+1)−1, and...
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, is_prime_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"
#include "../utils/pow2.h"

/* ------------------------------------------------------------------ */
/*  Return the number of witnesses for strong pseudoprimality of 2n+1  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* N = 2n + 1 (always odd) */
    seq_t N = (seq_t)n * 2 + 1;

    /* If N is prime, all bases are witnesses. */
    if (is_prime_small(N))
        return N - 1;

    /* Factor N (it fits into uint64_t for all practical n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)N, primes, exps, &cnt);

    /* ω(k) = number of distinct prime factors of N. */
    size_t omega = cnt;

    /* k1 = min_{p|N} v₂(p-1). */
    size_t min_v2 = (size_t)-1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pm1 = primes[i] - 1;
        size_t v2 = 0;
        while ((pm1 & 1ULL) == 0) { ++v2; pm1 >>= 1; }
        if (v2 < min_v2) min_v2 = v2;
    }

    /* Odd part of N-1. */
    uint64_t qodd = (uint64_t)N - 1;
    while ((qodd & 1ULL) == 0) qodd >>= 1;
    seq_t q = qodd;          /* q fits in seq_t because N is small */

    /* Compute the sum term: 1 + Σ_{j=0}^{k1-1} 2^{j·k}. */
    seq_t sum = 1;                           /* leading 1 */
    for (size_t j = 0; j < min_v2; ++j) {
        seq_t term = pow2((idx_t)(j * omega));
        sum += term;
    }

    /* Compute the product term: Π gcd(q, odd part of (p-1)). */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pm1 = primes[i] - 1;
        while ((pm1 & 1ULL) == 0) pm1 >>= 1; /* odd part of (p-1) */
        seq_t qi = pm1;
        prod *= gcd(q, qi);
    }

    return sum * prod;
}
