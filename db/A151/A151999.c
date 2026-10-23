/*
 * A151999  Numbers k such that every prime that divides phi(k) also divides
 *          k.
 * https://oeis.org/A151999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 2338, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_unique_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A151999: numbers k such that every
   prime dividing φ(k) also divides k.
   Search-based algorithm: iterate over natural numbers, testing the
   property by factoring each k and all p-1 for its prime factors. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_unique_u64.h"

/* Test whether k satisfies: every prime dividing φ(k) also divides k. */
static bool valid_k(uint64_t k)
{
    if (k == 1ULL) return true;

    uint64_t k_factors[16];
    size_t k_cnt = 0;
    factor_unique_u64(k, k_factors, &k_cnt);

    for (size_t i = 0; i < k_cnt; ++i) {
        uint64_t p = k_factors[i];
        if (p <= 2ULL) continue;          /* p-1 == 1 has no prime factors */
        uint64_t pminus_factors[16];
        size_t pcnt = 0;
        factor_unique_u64(p - 1ULL, pminus_factors, &pcnt);
        for (size_t j = 0; j < pcnt; ++j) {
            uint64_t q = pminus_factors[j];
            if (k % q != 0ULL) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t k = 1ULL;
    while (1) {
        if (valid_k(k)) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
