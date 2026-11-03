/*
 * A118106  Period of the vector sequence d(n)^k mod n for k=1,2,3,..., where
 *          d(n) is the vector of divisors of n.
 * https://oeis.org/A118106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{1<=i,j<=t, i!=j} ord(p_i, p_j^e_j), where n = Π
 *            p_k^e_k are the prime power factorisation of n
 * Range:     u8 n <= 537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9940 (2026-11-01)
 * Helpers:   factor_u64, gcd, powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

/* factorisation of a 64‑bit integer into distinct prime factors */
static void _factor_unique(uint64_t n, uint64_t *factors, size_t *cnt)
{
    size_t c = 0;
    if (n % 2 == 0) {
        factors[c++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            factors[c++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) factors[c++] = n;
    *cnt = c;
}

/* multiplicative order of a modulo m (a and m coprime, m is prime power) */
static seq_t _order(seq_t a, seq_t m)
{
    /* phi(m) for a prime power m = p^e is m - m / p, but we can just
       use the provided calc_phi routine which is efficient enough. */
    seq_t phi_seq = totient_seq(m);
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
            if (powmod(a, (uint64_t)trial, m) == 1)
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
