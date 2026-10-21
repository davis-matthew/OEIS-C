/*
 * A071126  Length of least repunit which is a multiple of the n-th prime, or
 *          0 if no such multiple exists.
 * https://oeis.org/A071126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 961, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the length of the least repunit divisible by the n‑th prime,
   or 0 if no such multiple exists.  The length is the multiplicative
   order of 10 modulo 9·p, with p = prime(n).  For p = 2 or 5 no
   repunit is divisible by p, so the function returns 0.  */
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

seq_t a(idx_t n)
{
    /* n must be >=1; negative or zero indices are treated as 0. */
    if (n == 0) return (seq_t)0;

    /* Obtain the n‑th prime. */
    uint32_t p = get_prime(n);

    /* Primes 2 and 5 never divide any repunit. */
    if (p == 2u || p == 5u) return (seq_t)0;

    /* Modulus M = 9·p. */
    seq_t M = (seq_t)9 * (seq_t)p;

    /* Compute φ(M).  For p≠3, M = 9·p = 3²·p, so φ = φ(9)·φ(p) = 6·(p‑1).
       For p = 3, M = 27, φ = 18. */
    idx_t phi = (p == 3u) ? 18ULL : 6ULL * ((idx_t)p - 1ULL);

    /* Factor φ into distinct prime factors.  φ is at most 1.4 · 10⁶,
       so trial division is fast. */
    idx_t fac[32];
    size_t fac_cnt = 0;
    idx_t tmp = phi;
    for (idx_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            fac[fac_cnt++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) fac[fac_cnt++] = tmp;

    /* Find the multiplicative order of 10 modulo M by reducing φ
       by its prime factors as long as the congruence still holds. */
    idx_t order = phi;
    bool reduced;
    do {
        reduced = false;
        for (size_t i = 0; i < fac_cnt; ++i) {
            idx_t q = fac[i];
            while (order % q == 0) {
                idx_t cand = order / q;
                if (mod_pow((seq_t)10, (uint64_t)cand, M) == (seq_t)1) {
                    order = cand;
                    reduced = true;
                } else {
                    break;          /* cannot divide further by q */
                }
            }
        }
    } while (reduced);

    return (seq_t)order;
}
