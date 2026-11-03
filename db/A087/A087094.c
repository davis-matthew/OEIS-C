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
 * Validated: standard, pass, 61 terms through n = 7552 (2026-11-01)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
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
            if (powmod((seq_t)10, cand, mod) == (seq_t)1) {
                ord = cand;            /* successful reduction */
            } else {
                break;                 /* no further cancellation with this prime factor */
            }
        }
    }
    return (seq_t)ord;
}
