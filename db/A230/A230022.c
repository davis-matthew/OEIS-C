/*
 * A230022  a(n) = |{the number of primes in the interval (k*n, (k+1)*n]: k =
 *          0, 1, ..., n-1}|.
 * https://oeis.org/A230022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4000, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A230022: a(n) = |{#primes in (k*n,(k+1)*n] : k=0..n-1}| */

/* This function may be called in any order. It caches the prime list
   for the largest limit (n*n) already sieved, extending it only when
   necessary. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes.h"   /* build_primes(limit, &out, &cnt) */

static int cmp_uint32(const void *a, const void *b)
{
    const uint32_t *x = (const uint32_t *)a;
    const uint32_t *y = (const uint32_t *)b;
    return (*x < *y) - (*x > *y);
}

/* Cache of the largest sieve performed. */
static struct {
    idx_t      limit;          /* current maximal limit n*n */
    idx_t *    primes;         /* array of primes ≤ limit */
    size_t     count;          /* number of primes stored */
} prime_cache = {0, NULL, 0};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Limit up to which primes are needed: n*n */
    idx_t limit = n * n;

    /* Extend the cache only if required */
    if (limit > prime_cache.limit) {
        if (prime_cache.primes != NULL) free(prime_cache.primes);
        build_primes(limit, &prime_cache.primes, &prime_cache.count);
        prime_cache.limit = limit;
    }

    /* Allocate array for counts in each sub‑interval. */
    uint32_t *cnt = (uint32_t *)calloc((size_t)n, sizeof(uint32_t));
    if (!cnt) return (seq_t)0;  /* allocation failure */

    /* Count primes per interval. */
    for (size_t i = 0; i < prime_cache.count; ++i) {
        idx_t p = prime_cache.primes[i];
        if (p > limit) break;               /* no more relevant primes */
        idx_t k = (p - 1) / n;              /* interval index 0‑based */
        if (k < n) cnt[(size_t)k]++;        /* record this prime */
    }

    /* Sort counts to identify distinct values. */
    qsort(cnt, (size_t)n, sizeof(uint32_t), cmp_uint32);

    /* Count distinct values. */
    size_t distinct = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        if (i == 0 || cnt[i] != cnt[i - 1]) distinct++;
    }

    free(cnt);
    return (seq_t)distinct;
}
