/*
 * A123475  Product of the primitive roots of prime(n).
 * https://oeis.org/A123475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primitive roots modulo the n‑th prime
 *            (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 145 (2026-11-03)
 * Helpers:   factorize, gcd_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/factorize.h"
#include "../utils/gcd_u64.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t  *cache     = NULL;
    static bool   *computed  = NULL;
    static idx_t   cache_len = 0;

    if (n == 0)        /* unused domain – defensive */
        return seq_one();

    /* grow cache if required */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache;
        bool  *new_comp;

        new_cache = mem_resize(seq_t, cache, new_len);
        new_comp  = (bool *)realloc(computed, new_len * sizeof(bool));
        if (!new_cache || !new_comp) {
            mem_free(cache);
            free(computed);
            abort();
        }

        /* initialise new "computed" entries to false */
        memset(new_comp + cache_len, 0, (new_len - cache_len) * sizeof(bool));

        cache    = new_cache;
        computed = new_comp;
        cache_len = new_len;
    }

    /* return memoised value if available */
    if (computed[n])
        return cache[n];

    seq_t result;

    /* special cases */
    if (n == 1) {                     /* a(1) = 1 */
        result = seq_one();
    } else if (n == 2) {               /* a(2) = 2 */
        result = (seq_t)2;
    } else {
        /* the n-th prime */
        uint32_t p32 = get_prime((idx_t)n);
        seq_t    p   = (seq_t)p32;          /* prime as seq_t */

        /* factorisations of p‑1 (Euler phi) */
        uint64_t pf[64];
        int      exp[64];
        size_t   nf = factorize((uint64_t)p32 - 1, pf, exp);
        uint64_t m  = (uint64_t)p32 - 1;    /* φ(p) = p‑1 */

        /* find a primitive root g of the prime p */
        seq_t g = 0;
        for (seq_t cand = 2; cand < p; ++cand) {
            bool ok = true;
            for (size_t i = 0; i < nf; ++i) {
                uint64_t q = pf[i];
                if (powmod(cand, m / q, p) == ((seq_t)1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) { g = cand; break; }
        }
        if (g == 0)  /* safety, should not happen for p>2 */
            g = 2;

        /* product over all primitive roots: r = g^k (mod p) for k coprime to m */
        seq_t prod = seq_one();
        for (uint64_t k = 1; k < m; ++k) {
            if (gcd_u64(k, m) == 1) {
                seq_t r = powmod(g, k, p);
                prod = seq_mul(prod, r);
            }
        }
        result = prod;
    }

    /* store computed value */
    cache[n]    = result;
    computed[n] = true;
    return result;
}
