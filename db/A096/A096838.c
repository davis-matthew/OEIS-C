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
 * Helpers:   build_primes_upto, ceil_log_e, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/ceil_log_e.h"

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
