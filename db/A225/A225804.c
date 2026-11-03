/*
 * A225804  Arithmetic mean of the first n primes, rounded up.
 * https://oeis.org/A225804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( (p1 + p2 + … + pn) / n )
 * Range:     u8 n <= 13, u16 n <= 743, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 74 terms through n = 4570 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

/* Cumulative sums of primes, 0‑based. */
static seq_t *prime_sum_cache = NULL;
static size_t prime_sum_cap   = 0;   /* allocated number of slots */
static idx_t  prime_sum_cnt   = 0;   /* number of sums cached */

/* Ensure prime_sum_cache contains sums for the first n primes. */
static void
ensure_prime_sum_cache(idx_t n)
{
    if (n == 0)
        return;

    if (n <= prime_sum_cnt)
        return;          /* already cached */

    /* Grow the array if required. */
    if (prime_sum_cap < (size_t)n) {
        size_t new_cap = prime_sum_cap ? prime_sum_cap * 2 : 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = mem_resize(seq_t, prime_sum_cache, new_cap);
        prime_sum_cache = tmp;
        prime_sum_cap = new_cap;
    }

    /* Compute sums for the newly added primes. */
    seq_t last_sum = (prime_sum_cnt > 0) ? prime_sum_cache[prime_sum_cnt - 1] : 0;
    for (idx_t i = prime_sum_cnt; i < n; ++i) {
        uint32_t p = get_prime(i + 1);        /* i is 0‑based; get_prime is 1‑based */
        prime_sum_cache[i] = last_sum + (seq_t)p;
        last_sum = prime_sum_cache[i];
    }
    prime_sum_cnt = n;
}

/* Public function: arithmetic mean of the first n primes, rounded up. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1; returning 0 is safe */

    ensure_prime_sum_cache(n);
    seq_t sum = prime_sum_cache[n - 1];
    /* ceil division: (sum + n - 1) / n, all in seq_t */
    return (sum + (seq_t)n - 1) / (seq_t)n;
}
