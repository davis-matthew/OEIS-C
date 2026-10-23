/*
 * A174824  a(n) = period of the sequence {m^m, m >= 1} modulo n.
 * https://oeis.org/A174824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, λ(n)), where λ is Carmichael's function
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t lambda = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t k = exps[i];
        seq_t term;

        if (p == 2) {
            if (k == 1) {
                term = 1;                      /* λ(2)   = 1 */
            } else if (k == 2) {
                term = 2;                      /* λ(4)   = 2 */
            } else {
                term = pow_uint(2, k - 2);     /* λ(2^k) = 2^(k-2), k ≥ 3 */
            }
        } else {
            /* λ(p^k) = φ(p^k) = (p-1)*p^(k-1) for odd p */
            term = (seq_t)(p - 1) * pow_uint(p, k - 1);
        }

        seq_t g = gcd(lambda, term);
        lambda = (lambda / g) * term;
    }

    seq_t nseq = (seq_t)n;
    seq_t g = gcd(nseq, lambda);
    return (nseq / g) * lambda;
}
