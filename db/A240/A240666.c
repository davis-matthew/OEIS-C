/*
 * A240666  Least k such that k^m == -1 (mod prime(n)) has a solution for some
 *          m.
 * https://oeis.org/A240666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 7102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A240666: a(n) = least integer k ≥ 2 such that there is
   an exponent m with k^m ≡ –1 (mod p_n), where p_n is the n‑th prime (n ≥ 1).
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/get_prime.h"

/* factor a 64‑bit integer into distinct prime factors (small n only) */
static void _factor_uint64(uint64_t n, uint64_t *pr, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            pr[(*cnt)++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) pr[(*cnt)++] = n;
}

seq_t a(idx_t n)
{
    uint64_t p = get_prime(n);            /* n-th prime, n ≥ 1 */
    if (p == 2)                           /* -1 ≡ 1 (mod 2) → first odd k is 3 */
        return (seq_t)3;

    /* factor p‑1 */
    uint64_t fact[64];
    size_t    fcnt;
    _factor_uint64(p - 1, fact, &fcnt);

    for (uint64_t k = 2;; ++k) {
        uint64_t km = k % p;
        if (km == 0 || km == 1) continue;   /* 0^m or 1^m cannot be –1 mod p */

        /* compute multiplicative order of km modulo p */
        uint64_t order = p - 1;
        for (size_t i = 0; i < fcnt; ++i) {
            uint64_t q = fact[i];
            while (order % q == 0) {
                seq_t r = mod_pow((seq_t)km, order / q, (seq_t)p);
                if (r == (seq_t)1) {
                    order /= q;
                } else {
                    break;
                }
            }
        }
        if ((order & 1) == 0)                /* even order ⇒ k^m = –1 has a solution */
            return (seq_t)k;
    }
}
