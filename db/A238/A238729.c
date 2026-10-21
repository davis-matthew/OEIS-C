/*
 * A238729  Maximum flow of a number.
 * https://oeis.org/A238729
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{S⊆P(m), S≠∅} (∏_{i∉S} (e_i+1)) * (∑_{i∈S} p_i), *
 *            where n = ∏_{i=1..m} p_i^e_i. This computes the maximum flow in
 *            the * divisor network of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;          /* values below the offset are irrelevant */
    uint64_t nn = (uint64_t)n;

    /* factorization: primes and their exponents */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* iterate over all non‑empty subsets of the prime set */
    seq_t   best  = 0;
    bool    first = true;
    uint64_t subsets = 1ULL << cnt;   /* cnt ≤ 16 for 64‑bit n */

    for (uint64_t mask = 1; mask < subsets; ++mask) {
        seq_t prod = 1;
        seq_t sum  = 0;

        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                sum += (seq_t)primes[i];
            } else {
                prod *= (seq_t)(exps[i] + 1);
            }
        }

        seq_t val = prod * sum;
        if (first || val < best) {
            best  = val;
            first = false;
        }
    }
    return best;
}
