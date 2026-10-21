/*
 * A061884  a(n) = Sum_{ d | n } phi(lcm(d,n/d)), where phi(n) = Euler totient
 *          A000010.
 * https://oeis.org/A061884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = Sum_{ d|n } phi(lcm(d, n/d)) (OEIS A061884)
 * Range:     u8 n <= 84, u16 n <= 7314, u32 n <= 9374, u64 n <= 9374,
 *            u128 n <= 9374, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* not used, but safe */
    uint64_t nn = (uint64_t)n;

    /* factorization */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned  e = (unsigned)exps[i];

        /* sum_{k=0..e} max(p^k, p^(e-k)) */
        seq_t sum = 0;
        for (unsigned k = 0; k <= e; ++k) {
            seq_t pk   = pow_uint(p, k);
            seq_t pk_e = pow_uint(p, e - k);
            seq_t term = (pk > pk_e) ? pk : pk_e;
            sum += term;
        }

        /* component for this prime power: (p-1)/p * sum */
        seq_t component = (sum * (seq_t)(p - 1)) / (seq_t)p;
        result *= component;
    }

    return result;
}
