/*
 * A118106  Period of the vector sequence d(n)^k mod n for k=1,2,3,..., where
 *          d(n) is the vector of divisors of n.
 * https://oeis.org/A118106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{1<=i,j<=t, i!=j} ord(p_i, p_j^e_j), where n = Π
 *            p_k^e_k are the prime power factorisation of n
 * Range:     u8 n <= 537, u16 n <= 9940, u32 n <= 9940, u64 n <= 9940,
 *            u128 n <= 9940, big any n
 * Helpers:   _factor_unique, calc_phi, factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"
#include "../utils/_factor_unique.h"

/* multiplicative order of a modulo m (a and m coprime, m is prime power) */
static seq_t _order(seq_t a, seq_t m)
{
    /* phi(m) for a prime power m = p^e is m - m / p, but we can just
       use the provided calc_phi routine which is efficient enough. */
    seq_t phi_seq = calc_phi(m);
    uint64_t phi = (uint64_t)phi_seq;   /* phi <= 10000 for n <= 10000 */

    /* factorisation of phi to obtain its distinct prime divisors */
    uint64_t prime_factors[16];
    size_t pf_cnt = 0;
    _factor_unique(phi, prime_factors, &pf_cnt);

    seq_t ord = phi_seq;
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = prime_factors[i];
        while (ord % p == 0) {
            seq_t trial = ord / (seq_t)p;
            if (mod_pow(a, (uint64_t)trial, m) == 1)
                ord = trial;
            else
                break;
        }
    }
    return ord;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pcnt = 0;
    factor_u64(n64, primes, exps, &pcnt);

    if (pcnt <= 1)        /* prime power => period 1 */
        return (seq_t)1;

    seq_t lcm = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        /* compute modulus m_j = p_j^e_j */
        seq_t mj = 1;
        for (uint64_t k = 0; k < exps[i]; ++k)
            mj *= (seq_t)primes[i];

        for (size_t j = 0; j < pcnt; ++j) {
            if (i == j) continue;
            seq_t ord = _order((seq_t)primes[j], mj);
            seq_t g = gcd(lcm, ord);
            lcm = lcm / g * ord;
        }
    }
    return lcm;
}
