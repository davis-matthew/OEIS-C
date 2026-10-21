/*
 * A087094  a(n) = smallest k such that (10^k-1)/9 == 0 mod prime(n)^2, or 0
 *          if no such k exists.
 * https://oeis.org/A087094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if prime(n) = 2 or 5, 9 if prime(n) = 3, otherwise the
 *            multiplicative order of 10 modulo prime(n)^2. (This satisfies
 *            a(1)=0, a(2)=9, a(3)=0, a(4)=42, …)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_prime.h"        /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/factor_u64.h"       /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* special cases */
    if (p == 2u || p == 5u) return 0;   /* none exist */
    if (p == 3u)               return 9; /* given in the OEIS comments */

    /* modulus: p^2 */
    seq_t mod = (seq_t)p * (seq_t)p;

    /* Euler's totient of p^2: φ(p^2) = p·(p‑1) */
    uint64_t phi = (uint64_t)p * (uint64_t)(p - 1);

    /* factor φ(p^2) so that we can test divisors */
    uint64_t primes_factors[64];
    uint64_t exponents[64];
    size_t factcnt = 0u;
    factor_u64(phi, primes_factors, exponents, &factcnt);

    /* multiplicative order of 10 modulo mod */
    uint64_t ord = phi;                /* start with the maximal possible order */
    for (size_t i = 0u; i < factcnt; ++i) {
        uint64_t q = primes_factors[i];
        /* try to cancel powers of q from the current order */
        while (ord % q == 0u) {
            uint64_t cand = ord / q;
            if (mod_pow((seq_t)10, cand, mod) == (seq_t)1) {
                ord = cand;            /* successful reduction */
            } else {
                break;                 /* no further cancellation with this prime factor */
            }
        }
    }
    return (seq_t)ord;
}
