/*
 * A086000  For p = prime(n), a(n) is the smallest N such that pN is a base-2
 *          pseudoprime (that is, 2^(pN-1) = 1 mod pN).
 * https://oeis.org/A086000
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 778, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 68 terms through n = 9858 (2026-11-01)
 * Helpers:   build_spf, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For n>=2, a(n) is the smallest N such that p_n * N is a
 * base‑2 pseudoprime; p_n is the n‑th prime.  The algorithm computes N
 * as 1 + 2*ord(4,p_n)*k, incrementing k until the pseudoprime condition
 * holds. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"
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

/* static smallest prime factor table (prepared once) */
static idx_t *SPF = NULL;
static const idx_t SPF_LIMIT = 2000000;

static void ensure_spf(void)
{
    if (SPF == NULL) {
        build_spf(SPF_LIMIT, &SPF);
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    ensure_spf();

    /* prime p = p_n */
    uint32_t p_val = get_prime(n);      /* returns 32‑bit prime */
    seq_t p = (seq_t)p_val;

    /* compute ord_4(p) */
    uint64_t order = (uint64_t)p_val - 1;      /* start with p-1 */
    uint64_t temp  = order;
    while (temp > 1) {
        idx_t f = SPF[(idx_t)temp];
        if (f == 0) f = temp;                /* safety for 1 */
        /* reduce order by powers of f while 4^(order/f) ≡ 1 mod p */
        while (order % f == 0) {
            uint64_t candidate = order / f;
            if (powmod((seq_t)4, candidate, p) == (seq_t)1) {
                order = candidate;
            } else {
                break;
            }
        }
        /* remove factor f from temp */
        while (temp % f == 0) temp /= f;
    }

    /* search for the minimal N = 1 + 2*order*k */
    uint64_t k = 1;
    for (;;) {
        seq_t N = (seq_t)(1 + 2 * order * k);
        seq_t M = p * N;                 /* p * N  (composite odd) */
        uint64_t exp = (uint64_t)(M - 1); /* exponent fits in 64 bits */
        if (powmod((seq_t)2, exp, M) == (seq_t)1) {
            return N;
        }
        ++k;
    }
}
