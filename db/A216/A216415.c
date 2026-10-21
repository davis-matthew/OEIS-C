/*
 * A216415  a(n) = smallest positive m such that 2n-1 | 10^m-1, or 0 if no
 *          such m exists.
 * https://oeis.org/A216415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that 2n‑1 | 10^m‑1; 0 if such an m does
 *            not exist (equivalently the multiplicative order of 10 modulo
 *            2n‑1, 0 when gcd(10,2n‑1)≠1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   calc_phi, factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/calc_phi.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2 - 1;              /* modulus */
    seq_t g = gcd(m, (seq_t)10);             /* check coprimality */
    if (g != 1) return (seq_t)0;             /* no inverse → order undefined */

    seq_t phi = calc_phi(m);                 /* Euler’s totient of m */
    seq_t order = phi;                       /* current candidate for the order */

    /* factor phi to obtain its distinct prime divisors */
    uint64_t phi64 = (uint64_t)phi;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;
    factor_u64(phi64, primes, exps, &pcnt);

    /* refine the order by eliminating prime factors whenever possible */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        while (order % (seq_t)p == 0) {
            seq_t cand = order / (seq_t)p;
            if (mod_pow((seq_t)10, (uint64_t)cand, m) == 1)
                order = cand;                 /* p is redundant in the exponent */
            else
                break;
        }
    }
    return order;
}
