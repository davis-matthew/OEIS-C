/*
 * A212173  First integer with same second signature as n (cf. A212172).
 * https://oeis.org/A212173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = first integer with the same second signature as n
 *            computed by factorizing n and assigning its >=2 exponents to the
 *            smallest primes in reverse exponent order
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* factorize n (n is 64‑bit) */
    uint64_t primes_tmp[64];
    uint64_t exps_tmp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes_tmp, exps_tmp, &cnt);

    /* collect exponents >= 2 */
    idx_t exps[64];
    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps_tmp[i] >= 2) {
            exps[k++] = (idx_t)exps_tmp[i];
        }
    }
    if (k == 0) return (seq_t)1;  /* no exponents >=2 → answer is 1 */

    /* sort exponents descending (small k → simple sort) */
    for (size_t i = 0; i < k; ++i) {
        for (size_t j = i + 1; j < k; ++j) {
            if (exps[i] < exps[j]) {
                idx_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
        }
    }

    /* first 20 primes are enough for all n up to 2^64 */
    static const uint64_t small_primes[20] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL,
        37ULL, 41ULL, 43ULL, 47ULL, 53ULL,
        59ULL, 61ULL, 67ULL, 71ULL
    };

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < k; ++i) {
        seq_t pow = seq_pow((seq_t)small_primes[i], exps[i]);  /* small_primes[i]^(exps[i]) */
        result = seq_mul(result, pow);
    }
    return result;
}
