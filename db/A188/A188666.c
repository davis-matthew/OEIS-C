/*
 * A188666  Largest m <= n such that lcm(m, m+1, ..., n) = lcm(1, 2, ..., n).
 * https://oeis.org/A188666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest prime power > floor(n/2)
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _upper_bound_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_upper_bound_uint64.h"

/* Comparator for qsort on uint64_t. */
static int _cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Compute the sequence term. */
seq_t a(idx_t n)
{
    static idx_t pre_lim = 0;          /* Highest n that has been processed. */
    static uint64_t *prime_pows = NULL;/* Sorted list of prime powers <= pre_lim. */
    static size_t prime_pow_cnt = 0;   /* Number of entries in prime_pows. */

    /* Ensure we have precomputed up to the required n. */
    if (n > pre_lim) {
        /* Free previous data if any. */
        if (prime_pows) free(prime_pows);
        pre_lim = n;

        /* Simple sieve of Eratosthenes for primes up to pre_lim. */
        size_t max = (size_t)pre_lim;
        bool *is_prime = (bool*)malloc((max + 1) * sizeof(bool));
        for (size_t i = 0; i <= max; ++i) is_prime[i] = true;
        if (max >= 0) is_prime[0] = false;
        if (max >= 1) is_prime[1] = false;
        for (size_t i = 2; i * i <= max; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= max; j += i)
                    is_prime[j] = false;
            }
        }

        /* Estimate upper bound on number of prime powers to allocate. */
        size_t est = max / 2 + 20;          /* Rough guess */
        uint64_t *arr = (uint64_t*)malloc(est * sizeof(uint64_t));
        size_t cnt = 0;

        /* 1 is considered a prime power for the sequence definition. */
        arr[cnt++] = 1ULL;

        /* Generate prime powers. */
        for (size_t p = 2; p <= max; ++p) {
            if (!is_prime[p]) continue;
            uint64_t val = p;
            while (val <= (uint64_t)max) {
                arr[cnt++] = val;
                if (val > (uint64_t)max / p) break; /* avoid overflow */
                val *= p;
            }
        }

        free(is_prime);

        /* Sort and deduplicate. */
        qsort(arr, cnt, sizeof(uint64_t), _cmp_uint64);
        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || arr[i] != arr[i - 1])
                arr[uniq++] = arr[i];
        }
        arr = (uint64_t*)realloc(arr, uniq * sizeof(uint64_t));

        prime_pows   = arr;
        prime_pow_cnt = uniq;
    }

    /* Compute floor(n/2). */
    uint64_t t = (uint64_t)n >> 1;

    /* Find first prime power greater than t. */
    size_t idx = _upper_bound_uint64(prime_pows, prime_pow_cnt, t);
    /* idx should always be valid because a prime exists between t and 2t (Bertrand). */
    if (idx >= prime_pow_cnt) idx = prime_pow_cnt - 1;  /* safety guard */

    return (seq_t)prime_pows[idx];
}
