/*
 * A024530  Numerator of -Sum_{k=1..n} (-1)^k / prime(k).
 * https://oeis.org/A024530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 327 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024530: Numerator of -Sum_{k=1..n} (-1)^k / prime(k)
 *
 * Recurrence (for n ≥ 2):
 *     a(0) = 0,  a(1) = 1
 *     a(n+1) = a(n) * prime(n+1) + (-1)^n * primorial( prime(n) )
 * 
 * Where prime(k) is the k‑th prime and
 * primorial(p) = product of all primes ≤ p.
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

/* static cache for already computed a(n) */
static seq_t *cache      = NULL;   /* a(0) .. a(k) */
static size_t cache_len  = 0;      /* number of stored terms */

/* product of the first (cache_len-1) primes (i.e. primorial of prime(cache_len-1)) */
static seq_t primorial_prod = 1;

/* --------------------------------------------------------------------- *
 *  Ensure the cache contains terms a(0) .. a(n).  The primorial_prod is
 *  updated accordingly.
 * --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(cache_len - 1) >= n)        /* already computed */
        return;

    /* enlarge the cache */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_len);
    if (!tmp)            /* allocation failed – leave cache unchanged */
        return;
    cache = tmp;

    /* compute missing terms */
    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t term;
        if (i == 0) {
            term = 0;          /* a(0) */
        } else if (i == 1) {
            term = 1;          /* a(1) */
        } else {
            /* a(i) = a(i-1)*prime(i) + (-1)^(i-1) * primorial( prime(i-1) ) */
            seq_t prime_i   = (seq_t)get_prime((idx_t)i);
            seq_t prime_prev = (seq_t)get_prime((idx_t)(i - 1));

            if (((i - 1) & 1U) == 0)            /* even exponent → +1 */
                term = cache[i - 1] * prime_i + primorial_prod;
            else                               /* odd exponent  → -1 */
                term = cache[i - 1] * prime_i - primorial_prod;

            /* primorial_prod now becomes primorial of prime(i) for next step */
            primorial_prod *= prime_i;
        }
        cache[i] = term;
    }

    cache_len = new_len;
}

/* --------------------------------------------------------------------- *
 *  Public function for the sequence
 * --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (cache_len == 0) {
        /* initialise a(0) .. a(2) and primorial_prod = 2*3 */
        cache_len = 3;
        cache = mem_new(seq_t, cache_len);
        if (!cache) return 0;           /* allocation failure */

        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;

        primorial_prod = (seq_t)( (unsigned)get_prime(1) *
                                  (unsigned)get_prime(2) ); /* 2 * 3 */
    }

    ensure_cache(n);

    /* If allocation failed inside ensure_cache, cache[n] may be invalid.
       In that pathological case, return 0 as a safe fallback. */
    if ((idx_t)(cache_len - 1) < n)
        return 0;

    return cache[n];
}
