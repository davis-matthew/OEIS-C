/*
 * A219429  Highest prime primitive root (less than p) for the n-th prime p.
 *          (or 0 if none exists).
 * https://oeis.org/A219429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = highest prime primitive root less than the n‑th prime, or
 *            0 if none exists. This implementation builds all primes < p(n),
 *            factors p(n)-1 by simple trial division, and tests each
 *            candidate prime for the primitive‑root condition
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9674 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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

/* Return the desired term for index n (1‑based).  The implementation
   works for any seq_t/idx_t types defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1 – return 0 for non‑positive indices */
        return 0;

    /* the n‑th prime, guaranteed to fit in uint32_t for n <= 10 000 */
    uint32_t p = get_prime(n);
    if (p < 2)
        return 0;

    /* factor p‑1 into its distinct prime factors */
    uint64_t m = (uint64_t)p - 1;
    uint64_t pf[64];
    size_t pf_cnt = 0;

    if (m % 2 == 0) {
        pf[pf_cnt++] = 2;
        while (m % 2 == 0) m /= 2;
    }
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            pf[pf_cnt++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) pf[pf_cnt++] = m;

    /* build all primes <= p-1; primes array will contain p-1 or smaller */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(p-1, &primes, &primes_cnt);

    seq_t best = 0;  /* default if no prime primitive root exists */

    /* search descending for the largest prime that is a primitive root mod p */
    for (long long idx = (long long)primes_cnt - 1; idx >= 0; --idx) {
        uint64_t q = primes[(size_t)idx];
        bool ok = true;
        for (size_t j = 0; j < pf_cnt; ++j) {
            uint64_t f = pf[j];
            if (powmod((seq_t)q, (p - 1) / f, (seq_t)p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            best = (seq_t)q;
            break;  /* highest found */
        }
    }

    free(primes);
    return best;
}
