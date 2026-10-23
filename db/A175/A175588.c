/*
 * A175588  a(1) = 4; a(n) = smallest semiprime > a(n-1) such that a(n) -
 *          a(n-1) is semiprime.
 * https://oeis.org/A175588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest semiprime > a(n-1) such that the difference * is
 *            also a semiprime; a(1) = 4
 * Range:     u8 n <= 28, u16 n <= 6589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Integer square root for uint64_t, using binary search. */
static uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = n, mid;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else if (mid == 0) break;
        else high = mid - 1;
    }
    return high;
}

/* Dynamically maintained list of primes for factorisation. */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0, prime_len = 0;

/* Ensure that prime_list contains all primes <= limit. */
static void ensure_primes_upto(uint64_t limit) {
    if (prime_len == 0) {
        /* initialise with the first prime 2 */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        }
        prime_list[0] = 2;
        prime_len = 1;
    }
    uint64_t start = prime_list[prime_len - 1] + 1;
    if (prime_len != 1) {
        if (start % 2 == 0) start++;   /* skip even numbers after 2 */
    } else {
        if (start < 3) start = 3;
    }
    for (uint64_t cand = start; cand <= limit; cand += 2) {
        bool is_p = true;
        uint64_t root = isqrt_u64(cand);
        for (size_t i = 0; i < prime_len && prime_list[i] <= root; ++i) {
            if (cand % prime_list[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            /* extend array if needed */
            if (prime_len == prime_cap) {
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;
        }
    }
}

/* Check if x (uint64_t) is a semiprime (product of exactly two primes,
 * multiplicities counted). */
static bool is_semiprime_uint(uint64_t x) {
    if (x < 4) return false;
    int total = 0;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        while (x % p == 0) {
            ++total;
            x /= p;
            if (total > 2) return false;
        }
    }
    if (x > 1) ++total;   /* remaining prime factor */
    return total == 2;
}

/* Cache for computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, but safe guard */
    /* Extend cache if needed. */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {
            /* initialise cache with first term 4 */
            cache_len = 1;
            seq_cache = (seq_t *)malloc(sizeof(seq_t));
            seq_cache[0] = (seq_t)4;
            /* initialise prime list up to sqrt(4)=2 */
            ensure_primes_upto(2);
            continue;
        }
        seq_t last = seq_cache[cache_len - 1];
        seq_t cand = last + 1;
        while (1) {
            uint64_t cand64 = (uint64_t)cand;
            ensure_primes_upto(isqrt_u64(cand64));
            if (!is_semiprime_uint(cand64)) {
                ++cand;
                continue;
            }
            uint64_t diff = cand64 - (uint64_t)last;
            ensure_primes_upto(isqrt_u64(diff));
            if (is_semiprime_uint(diff)) break;
            ++cand;
        }
        /* Store the new term. */
        cache_len++;
        seq_cache = (seq_t *)realloc(seq_cache, cache_len * sizeof(seq_t));
        seq_cache[cache_len - 1] = cand;
    }
    return seq_cache[n - 1];
}
