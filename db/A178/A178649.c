/*
 * A178649  a(n) = product of nonsquarefree divisors of n.
 * https://oeis.org/A178649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^{τ(n)/2} / rad(n)^{2^{ω(n)-1}} where τ(n) is the
 *            divisor count, ω(n) the number of distinct prime factors, and
 *            rad(n) the product of distinct primes
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 95, u64 n <= 287,
 *            u128 n <= 1007, big any n
 * Helpers:   big_pow, factor_u64, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* a(1) = 1 (no nonsquarefree divisors) */
    if (n == (idx_t)1) return seq_one();

    /* Factor n into primes and exponents. */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = ∏ (e_i + 1). */
    uint64_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (uint64_t)(exps[i] + 1);

    /* 2^{ω(n)-1}  (ω = cnt).  For cnt==0 (n==1) it would be 1. */
    uint64_t pow2_omega = (cnt > 0) ? (1ULL << (cnt - 1)) : 1ULL;

    seq_t result = seq_one();

    for (size_t i = 0; i < cnt; ++i) {
        /* Exponent for this prime in the numerator:  e_i * τ(n) / 2.  */
        unsigned __int128 prod = (unsigned __int128)exps[i] * tau;
        unsigned __int128 exp128 = prod >> 1;            /* divide by 2 */
        exp128 -= pow2_omega;                            /* subtract denominator part */

        /* Convert to idx_t for big_pow (exponent fits into 64‑bit for the test range). */
        idx_t exponent = (idx_t)exp128;

        seq_t prime_pow = big_pow((seq_t)primes[i], (seq_t)exponent);
        result = seq_mul(result, prime_pow);
    }
    return result;
}
