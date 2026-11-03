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
 * Validated: standard, pass, 103 terms through n = 9454 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A240666: a(n) = least integer k ≥ 2 such that there is
   an exponent m with k^m ≡ –1 (mod p_n), where p_n is the n‑th prime (n ≥ 1).
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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
                seq_t r = powmod((seq_t)km, order / q, (seq_t)p);
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
