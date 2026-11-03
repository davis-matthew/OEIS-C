/*
 * A116617  Values of n such that prime(2n+1) mod 12 = 11.
 * https://oeis.org/A116617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n such that prime(2*n+1) ≡ 11 (mod 12)
 * Range:     u8 n <= 9, u16 n <= 953, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t candidate    = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for offset 0 */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find the next n that satisfies the condition */
        while (1) {
            seq_t prime = (seq_t)get_prime(2 * candidate + 1);
            if ((prime % 12) == 11) break;   /* condition met */
            candidate++;                     /* test next integer */
        }

        /* ensure capacity and store the found candidate */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
        candidate++;                         /* start next search after this n */
    }

    return cache[n - 1];   /* sequences are 1‑based */
}
