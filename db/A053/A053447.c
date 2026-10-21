/*
 * A053447  Multiplicative order of 4 mod 2n+1.
 * https://oeis.org/A053447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 4 modulo (2n+1)
 * Range:     u8 n <= 234, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Return the smallest k>0 such that 4^k ≡ 1 (mod 2n+1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* by definition a(0)=1 */

    /* Work with 64‑bit integers because n≤10000 in the tests */
    uint64_t M = (uint64_t)n * 2 + 1;      /* 2n+1 */

    /* Compute φ(M) */
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_u64(M, primes, exps, &cnt);

    uint64_t phi = M;
    for (size_t i = 0; i < cnt; ++i)
        phi = phi / primes[i] * (primes[i] - 1);

    /* Factor φ(M) to get its prime factors */
    uint64_t ppr[32], pex[32];
    size_t pcnt;
    factor_u64(phi, ppr, pex, &pcnt);

    /* Reduce the candidate order using its prime factors */
    uint64_t order = phi;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = ppr[i];
        while (order % p == 0) {
            uint64_t cand = order / p;
            seq_t modpow = mod_pow((seq_t)4, cand, (seq_t)M);
            if (modpow == 1)
                order = cand;
            else
                break;
        }
    }

    return (seq_t)order;
}
