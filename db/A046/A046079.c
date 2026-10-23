/*
 * A046079  Number of Pythagorean triangles with leg n.
 * https://oeis.org/A046079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for odd n: a(n) = (τ(n²)–1)/2, for even n: a(n) =
 *            (τ((n/2)²)–1)/2 where τ(m) counts the positive divisors of m
 * Range:     u8 n <= 9298, u16 n <= 9298, u32 n <= 9298, u64 n <= 9298,
 *            u128 n <= 9298, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Return the number of Pythagorean triangles having leg n (OEIS A046079). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined but harmless */

    /* Determine the integer whose square we factor: n if odd, n/2 if even. */
    uint64_t m = (n & 1) ? (uint64_t)n : ((uint64_t)n) / 2ULL;

    /* Factor m into primes. */
    /* 64‑bit integers have at most 20 distinct prime factors. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    /* Compute τ(m²) = ∏(2·exp + 1). */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t add = 2 * (seq_t)exps[i] + 1;
        tau *= add;
    }

    /* Return (tau-1)/2. */
    return (tau - 1) / 2;
}
