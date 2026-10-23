/*
 * A183093  a(n) = number of divisors d of n such that d > 1 and if d =
 *          Product_(i) (p_i^e_i) then e_i = 1 for at least one i.
 * https://oeis.org/A183093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) – Π max(1, eᵢ) (eᵢ are the prime exponents of n)
 * Range:     u8 n <= 9965, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* τ(n) = number of divisors of n */
/* Π max(1, eᵢ) = number of powerful divisors of n (including 1) */
#include <stdint.h>
#include "../utils/factor_u64.h"

/* returns number of divisors d of n with d > 1 and at least one prime
   exponent equal to 1 (i.e. non‑powerful divisors > 1) */
seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 */
        return (seq_t)0;

    /* factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* τ(n) = Π (eᵢ + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Π max(1, eᵢ)  counts powerful divisors of n (including 1) */
    seq_t powerful = 1;
    for (size_t i = 0; i < cnt; ++i)
        powerful *= (seq_t)(exps[i] >= 2 ? exps[i] : 1);

    /* non‑powerful divisors > 1 */
    return tau - powerful;
}
