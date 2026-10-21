/*
 * A078458  Total number of factors in a factorization of n into Gaussian
 *          primes.
 * https://oeis.org/A078458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e∥n} e·( 2 if p = 2 or p ≡ 1 (mod 4); 1 otherwise )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0, a(0) is irrelevant */
        return 0;

    uint64_t nn = (uint64_t)n;
    /* maximum distinct prime factors for a 64‑bit integer is < 64 */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t add = (p == 2 || (p & 3ULL) == 1ULL) ? 2 : 1;
        result += (seq_t)exps[i] * add;
    }
    return result;
}
