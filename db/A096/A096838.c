/*
 * A096838  Number of primes in the neighborhood of prime(n), the n-th prime
 *          in the center with radius ceiling(log(prime(n))).
 * https://oeis.org/A096838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(p+ceil(log p)) - pi(p-ceil(log p)-1), where p =
 *            prime(n), ceil(log p) uses natural log, natural numbers, and
 *            a(1) corresponds to the first prime (2)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

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

/* Compute ceil(log(p)) for natural log, using long double. */
static uint64_t ceil_log_e(uint64_t p)
{
    long double val = log((long double)p);  /* natural log */
    return (uint64_t)ceill(val);
}

/* Binary search: first index with prime >= value. */
static size_t lower_bound(const uint64_t *arr, size_t n, uint64_t value)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Binary search: last index with prime <= value. */
static size_t upper_bound(const uint64_t *arr, size_t n, uint64_t value)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] <= value)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return SIZE_MAX;           /* no element <= value, should not occur here */
    return lo - 1;                 /* last index <= value */
}

/* Cache for primes up to the current maximum limit. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence starts at offset 1; undefined for 0 */
        return 0;

    /* Retrieve the n‑th prime (1‑indexed). */
    uint64_t p = (uint64_t)get_prime(n);

    /* Radius of the neighbourhood: ceil(log(p)). */
    uint64_t radius = ceil_log_e(p);

    /* Upper limit for the sieve: p + radius. */
    idx_t limit = p + radius;

    /* Rebuild prime cache if the required limit exceeds the current one. */
    if (limit > prime_cache_limit) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_cnt = 0;
        build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
        prime_cache_limit = limit;
    }

    /* Define window [p-radius, p+radius] respecting the prime domain. */
    uint64_t left  = (p > radius) ? (p - radius) : 2;   /* primes start at 2 */
    uint64_t right = p + radius;

    /* Locate indices of primes within the interval. */
    size_t first = lower_bound(prime_cache, prime_cache_cnt, left);
    size_t last  = upper_bound(prime_cache, prime_cache_cnt, right);

    /* Count the primes; should be at least one (the centre prime itself). */
    seq_t count;
    if (first <= last)
        count = (seq_t)(last - first + 1);
    else
        count = 0;      /* safety; should not happen */

    return count;
}
