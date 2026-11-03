/*
 * A062117  Order of 3 mod n-th prime.
 * https://oeis.org/A062117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9735 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative order of 3 modulo the nth prime
   (OEIS A062117: a(1)=1, a(2)=0) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/powmod.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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
            if (powmod((seq_t)3, (uint64_t)cand, (seq_t)p) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }
    return order;
}
