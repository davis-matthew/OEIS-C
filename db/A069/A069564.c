/*
 * A069564  a(1) = 2; a(n) = k*a(n-1) + 1 is a multiple of n-th prime with k >
 *          1.
 * https://oeis.org/A069564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 2; for n > 1, a(n) = k * a(n-1) + 1 where k > 1 is the
 *            smallest integer such that a(n) is divisible by the nth prime
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: full, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */

    static seq_t *cache = NULL;      /* cached terms, 1‑indexed */
    static size_t cache_cap = 0;     /* allocated size (in seq_t units) */
    static idx_t cached_n = 0;       /* number of terms stored (>= 0) */

    /* Make sure the cache can hold at least n terms */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise first term if this is the first call */
    if (cached_n == 0) {
        cache[0] = (seq_t)2;         /* a(1) = 2 */
        cached_n   = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];   /* a(i‑1) */

        /* Failure propagate: if previous term is already 0, keep 0 */
        if (prev == 0) {
            cache[i - 1] = (seq_t)0;
            continue;
        }

        uint32_t p = get_prime(i);   /* ith prime (i‑indexed, 1 → 2) */

        seq_t rem = prev % (seq_t)p; /* remainder of a(i‑1) mod p */

        /* If a(i‑1) is already divisible by p, no suitable k exists */
        if (rem == 0) {
            cache[i - 1] = (seq_t)0;
            continue;
        }

        /* Compute the modular inverse of rem modulo p */
        seq_t inv = powmod(rem, (idx_t)p - 2, (seq_t)p);

        /* Minimal solution of k*rem ≡ p−1 (mod p) */
        seq_t k0 = ((seq_t)(p - 1) * inv) % (seq_t)p;
        seq_t k  = k0;
        if (k0 <= (seq_t)1)           /* need k > 1 */
            k += (seq_t)p;

        /* a(i) = k * a(i‑1) + 1 */
        cache[i - 1] = k * prev + (seq_t)1;
    }

    cached_n = n;
    return cache[n - 1];
}
