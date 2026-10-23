/*
 * A116550  The bi-unitary analog of Euler's totient function of n.
 * https://oeis.org/A116550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) + Σ_{∅≠T⊆P} (-1)^{|T|} φ( ⌊(n-1)/∏_{p∈T}p^{e(p)}⌋ ,
 *            ∏_{p∈T}p )
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ(x,g) = #{1≤k≤x : gcd(k,g)=1} = Σ_{d|g} μ(d) ⌊x/d⌋, μ are the Möbius values. */

/* Implementation of OEIS A116550 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t plen = 0;
    factor_u64((uint64_t)n, primes, exps, &plen);

    /* compute prime powers p^e */
    uint64_t p_pow[64];
    for (size_t i = 0; i < plen; ++i) {
        uint64_t val = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            val *= primes[i];
        }
        p_pow[i] = val;
    }

    /* pre‑compute d_T = ∏ p^e and rad_T = ∏ p for all subsets of primes */
    size_t maxMask = ((size_t)1) << plen;
    uint64_t *dPow = (uint64_t *)malloc(maxMask * sizeof(uint64_t));
    uint64_t *rad  = (uint64_t *)malloc(maxMask * sizeof(uint64_t));
    if (!dPow || !rad) {
        /* fallback: large memory allocation failed */
        return (seq_t)0;
    }

    dPow[0] = 1;
    rad[0]  = 1;
    for (size_t mask = 1; mask < maxMask; ++mask) {
        size_t low = mask & -mask;
        size_t idx = __builtin_ctzll(low);
        size_t prev = mask ^ low;
        dPow[mask] = dPow[prev] * p_pow[idx];
        rad[mask]  = rad[prev] * primes[idx];
    }

    /* inclusion–exclusion sum */
    __int128 sum = ( __int128)(n - 1);   /* start with (n-1) */
    for (size_t mask = 1; mask < maxMask; ++mask) {
        uint64_t d = dPow[mask];
        uint64_t g = rad[mask];
        uint64_t x = ((uint64_t)(n - 1)) / d;   /* floor((n-1)/d) */

        /* compute φ(x,g) = Σ_{sub⊆mask} μ(sub) * floor(x / rad[sub]) */
        int64_t phi = 0;
        size_t sub = mask;
        while (1) {
            uint64_t prod = rad[sub];
            int subParity = (__builtin_popcountll(sub) & 1) ? -1 : 1;
            phi += subParity * (int64_t)(x / prod);
            if (sub == 0) break;
            sub = (sub - 1) & mask;
        }

        int maskParity = ((__builtin_popcountll(mask) & 1) ? -1 : 1);
        sum += ( (__int128)maskParity * ( __int128)phi );
    }

    free(dPow);
    free(rad);
    return (seq_t)sum;
}
