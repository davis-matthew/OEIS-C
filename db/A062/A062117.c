/*
 * A062117  Order of 3 mod n-th prime.
 * https://oeis.org/A062117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative order of 3 modulo the nth prime
   (OEIS A062117: a(1)=1, a(2)=0) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"        /* get_prime */
#include "../utils/build_primes_upto.h"/* build_primes_upto */
#include "../utils/mod_pow.h"          /* mod_pow */

seq_t a(idx_t n)
{
    /* 1‑based index: get the nth prime */
    uint32_t p = get_prime(n);

    /* Special cases: 2 → order 1, 3 → no multiplicative order */
    if (p == 2u) return (seq_t)1;
    if (p == 3u) return (seq_t)0;

    /* Order divides p‑1.  Start with p‑1. */
    seq_t order = (seq_t)(p - 1);

    /* Cache of small primes for factorising order,
       built lazily on first call. */
    static uint64_t *prime_tbl = NULL;
    static size_t prime_cnt   = 0;
    if (!prime_tbl) {
        idx_t limit = 200000;               /* > sqrt(2·10^5) */
        build_primes_upto(limit, &prime_tbl, &prime_cnt);
    }

    /* Extract distinct prime factors of order. */
    uint64_t tmp = (uint64_t)order;
    seq_t pf[64];
    size_t pf_len = 0;

    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= tmp; ++i) {
        uint64_t q = prime_tbl[i];
        if (tmp % q == 0) {
            pf[pf_len++] = (seq_t)q;
            while (tmp % q == 0)
                tmp /= q;
        }
    }
    if (tmp > 1) pf[pf_len++] = (seq_t)tmp;

    /* Reduce the order by prime factors while 3^order ≡ 1 (mod p). */
    for (size_t i = 0; i < pf_len; ++i) {
        seq_t q = pf[i];
        while (order % q == 0) {
            seq_t cand = order / q;
            if (mod_pow((seq_t)3, (uint64_t)cand, (seq_t)p) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }
    return order;
}
