/*
 * A108839  a(1) = 1; a(n) = number of previous terms a(k) such that a(k) + n
 *          is prime.
 * https://oeis.org/A108839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = number of k < n such that a(k)+n is
 *            prime
 * Range:     u8 n <= 1410, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime, seq_malloc, seq_realloc, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic cache for the sequence and a sparse
 * sieve for primality testing up to the largest value needed.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"      /* bool is_prime(uint64_t n); */
#include "../utils/seq_malloc.h"    /* seq_t *seq_malloc(idx_t n); */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t *p, idx_t n); */
#include "../utils/simple_sieve.h"  /* void simple_sieve(uint32_t limit, uint32_t **primes_out, size_t *count_out); */

/* Prime sieve data */
static uint32_t *prime_list = NULL;
static size_t  prime_count = 0;
static uint32_t sieve_limit = 0;

/* Extend the prime list up to 'limit'.  The list is rebuilt entirely at
 * each extension; this is sufficiently fast for the small limits needed.
 */
static void extend_primes(uint32_t limit)
{
    if (limit <= sieve_limit)
        return;

    if (prime_list)
        free(prime_list);

    simple_sieve(limit, &prime_list, &prime_count);
    sieve_limit = limit;
}

/* Binary search in the prime list (which contains only values <= sieve_limit). */
static bool is_prime_from_list(uint32_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] == x)
            return true;
        if (prime_list[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Prime test that uses the cached list for values that are small enough; otherwise
 * falls back to the generic 64‑bit test.  The sequence values are always small
 * enough to fit in a 64‑bit integer for the intended range of n.
 */
static bool is_prime_cached(uint64_t x)
{
    if (x <= sieve_limit)
        return is_prime_from_list((uint32_t)x);
    return is_prime(x);
}

/* Core sequence function */
seq_t a(idx_t n)
{
    /* OEIS offset 1: n = 0 is not used but we guard against it. */
    if (n == 0)
        return 0;

    /* Static state: the computed sequence cache, its size and the
     * number of terms calculated so far.  Also keep track of the
     * maximum value seen to bound the prime sieve.
     */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t computed    = 0;
    static seq_t max_value   = 0;

    /* Allocate or grow the cache as needed.  The cache always holds
     * a(1)..a(n) in indices 0..n-1.
     */
    if (!cache) {
        cache_size = (n > 0) ? n : 1;
        cache = seq_malloc(cache_size);
        if (!cache)
            return 0;
    } else if (n > cache_size) {
        seq_t *tmp = seq_realloc(cache, n);
        if (!tmp) {          /* realloc failed – abort safely */
            free(cache);
            cache = NULL;
            cache_size = 0;
            computed = 0;
            return 0;
        }
        cache = tmp;
        cache_size = n;
    }

    /* Initialise first term if this is the first call or the cache
     * has just been built.
     */
    if (computed == 0) {
        cache[0] = (seq_t)1;    /* a(1) = 1 */
        max_value = 1;
        computed = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* The largest value we will test for primality is
         * a(i-1)+i.  We extend the sieve up to a safe upper bound
         * that grows with i and the current maximum value seen.
         */
        uint32_t limit = (uint32_t)(i + (idx_t)max_value + 10);
        extend_primes(limit);

        seq_t count = 0;
        for (idx_t k = 1; k < i; ++k) {
            uint64_t val = (uint64_t)cache[k - 1] + (uint64_t)i;
            if (is_prime_cached(val))
                ++count;
        }
        cache[i - 1] = count;
        if (count > max_value)
            max_value = count;
    }

    computed = n;
    return cache[n - 1];
}
