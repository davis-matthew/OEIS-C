/*
 * A065969  a(n) = n^2 * Sum_{primes p dividing n} (1 + 1/p^2).
 * https://oeis.org/A065969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * Sum_{p|n} (1 + 1/p^2)
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1ULL) {
        return 0;
    }
    /* Arrays for distinct prime factors and their exponents.
       64 elements is enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 0;
    seq_t n_sq = (seq_t)n * (seq_t)n;   /* n^2 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t p_sq = (seq_t)p * (seq_t)p; /* p^2 */
        seq_t factor = n_sq / p_sq;          /* n^2 / p^2  */
        seq_t term = factor * (p_sq + (seq_t)1); /* (n^2/p^2)*(p^2+1) */
        res += term;
    }
    return res;
}
