/*
 * A087320  Smallest perfect power (at least a square) that is a multiple of
 *          n.
 * https://oeis.org/A087320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_power_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A087320 – Smallest perfect power (at least a square) that is a multiple of n.
 *
 *  Algorithm:
 *      The value a(n) is the minimum over k ≥ 2 of
 *          M(k) = ∏ p_i^(ceil(e_i / k) * k)
 *      where n = ∏ p_i^e_i.
 *      The exponent k that yields the minimal M(k) gives a(n).
 *      The loop over k only needs to go up to the largest exponent in the
 *      factorisation of n (or to 2 if all exponents are 1).
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_power_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1)=1; a(0) is irrelevant */

    /* factorise n */
    uint64_t primes[32];
    uint64_t exps   [32];
    size_t  cnt     = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned max_exp = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > max_exp) max_exp = (unsigned)exps[i];

    unsigned k_start = 2;
    unsigned k_end   = (max_exp > 1) ? max_exp : 2;

    seq_t best = 0;                     /* 0 means "not set yet" */

    for (unsigned k = k_start; k <= k_end; ++k) {
        bool overflow = false;
        seq_t cand = 1;

        for (size_t i = 0; i < cnt; ++i) {
            unsigned ceil_e = (exps[i] + k - 1) / k;   /* ceil(e_i / k) */
            unsigned f     = ceil_e * k;              /* exponent in M(k) */

            if (sizeof(seq_t) == 8) {                 /* 64‑bit build: avoid overflow */
                uint64_t pow64;
                if (!prime_power_uint64(primes[i], f, &pow64)) {
                    overflow = true; break;
                }
                __uint128_t temp = (__uint128_t)cand * pow64;
                if (temp > UINT64_MAX) {
                    overflow = true; break;
                }
                cand = (uint64_t)temp;
            } else {                                 /* arbitrary‑precision or larger bit-width */
                /* compute prime^f in seq_t safely by repeated multiply */
                seq_t pow_val = 1;
                for (unsigned j = 0; j < f; ++j) pow_val *= (seq_t)primes[i];
                cand *= pow_val;
            }

            if (!overflow && best && cand >= best) break;  /* early exit */
        }

        if (overflow) continue;
        if (!best || cand < best) best = cand;
    }

    return best;
}
