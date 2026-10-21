/*
 * A218147  Degree of minimal polynomial satisfied by
 *          exp(8*Pi*phi_2(1/n,1/n)), where phi_2 is defined in the Comments.
 * https://oeis.org/A218147
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   For an odd prime p: if p ≡ 1 (mod 4) → a(p) = ((p‑1)/2)² if p ≡
 *            3 (mod 4) → a(p) = ((p²‑1)/4) For a power of 2: a(2^e) =
 *            2^(2e‑3) (e≥1; e=1 gives 1/2, cancelled by the 4‑factor) For a
 *            general n = ∏ p_i^{e_i} (prime factorisation), let t = number of
 *            distinct primes dividing n. a(n) = 4^(t‑1) · ∏ a...
 * Range:     u8 n <= 33, u16 n <= 516, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Undefined below the offset (n < 3); return 0 */
    if (n < 3) return 0;

    uint64_t n64 = (uint64_t)n;

    /* Factorisation: at most 20 distinct prime factors for any 64‑bit number */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    /* Count distinct primes */
    size_t t = cnt;

    /* Compute 4^(t-1) (i.e. 2^(2*(t-1))) */
    seq_t numer = (seq_t)1 << (2 * (t - 1));
    seq_t denom = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            if (e == 1) {
                /* a(2) = 1/2; the factor 1/2 will be cancelled later by the 4‑factor */
                denom *= 2;
            } else {
                /* a(2^e) = 2^(2e-3) for e ≥ 2 */
                numer *= (seq_t)1 << (2 * e - 3);
            }
        } else {
            /* Compute a(p) for odd prime p */
            seq_t ap;
            if ((p & 3ULL) == 1ULL) {      /* p ≡ 1 (mod 4) */
                uint64_t k = (p - 1) >> 1; /* (p-1)/2 */
                ap = (seq_t)k * (seq_t)k;
            } else {                        /* p ≡ 3 (mod 4) */
                uint64_t k1 = (p - 1) >> 1; /* (p-1)/2 */
                uint64_t k2 = (p + 1) >> 1; /* (p+1)/2 */
                ap = (seq_t)k1 * (seq_t)k2;
            }

            /* For e > 1 multiply by p^{2(e-1)} */
            if (e > 1) {
                seq_t pe_pow = pow_uint(p, 2 * (e - 1));
                ap *= pe_pow;
            }

            numer *= ap;
        }
    }

    /* Final result: division exact as denom must cancel */
    return numer / denom;
}
