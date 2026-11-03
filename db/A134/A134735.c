/*
 * A134735  Primes followed by the difference from the next prime.
 * https://oeis.org/A134735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   if n is odd: a(n)=A000040((n+1)/2) else: a(n)=A001223(n/2)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9528 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;

static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp) abort(); /* out of memory */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

static void ensure_prime_count(idx_t needed)
{
    if ((size_t)needed <= prime_len) return;
    /* grow cache to required number of primes */
    ensure_prime_capacity((size_t)needed);
    for (size_t i = prime_len; i < (size_t)needed; ++i) {
        prime_cache[i] = (seq_t)get_prime((idx_t)(i + 1));
    }
    prime_len = (size_t)needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, guard against misuse */

    if ((n & 1) == 1) {                /* odd n: return nth prime */
        idx_t k = (n + 1) / 2;
        ensure_prime_count(k);
        return prime_cache[(size_t)(k - 1)];
    } else {                            /* even n: difference of consecutive primes */
        idx_t k   = n / 2;              /* we need primes up to index k+1 */
        idx_t need = k + 1;
        ensure_prime_count(need);
        return prime_cache[(size_t)k] - prime_cache[(size_t)(k - 1)];
    }
}
