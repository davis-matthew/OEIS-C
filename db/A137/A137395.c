/*
 * A137395  a(1)=1. a(n) = a(n-1) + (sum of the distinct primes that are <= n
 *          and don't divide a(n-1)).
 * https://oeis.org/A137395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + sum of distinct primes <= n that do
 *            not divide a(n-1)
 * Range:     u8 n <= 15, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static seq_t *a_cache = NULL;   /* a_cache[0] = a(1) */
static size_t a_size = 0;       /* number of elements currently stored */
static size_t a_alloc = 0;      /* allocated capacity for a_cache */

static uint64_t *primes = NULL; /* array of primes <= current maximum */
static size_t primes_cnt = 0;   /* number of primes in the array */

/* Ensure that primes array contains all primes <= limit */
static void ensure_primes(uint64_t limit)
{
    if (primes_cnt > 0 && primes[primes_cnt - 1] >= limit)
        return;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    if (primes != NULL)
        free(primes);

    primes = new_primes;
    primes_cnt = new_cnt;
}

/* Increase allocation for a_cache if needed */
static void ensure_a_cache_size(size_t needed)
{
    if (a_alloc >= needed)
        return;
    size_t new_alloc = needed * 2 > 1 ? needed * 2 : 2;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_alloc * sizeof(seq_t));
    a_cache = new_cache;
    a_alloc = new_alloc;
}

/* Compute a(1) .. a(n) and store them in a_cache */
static void compute_a_up_to(idx_t n)
{
    if (a_size >= n)
        return;

    /* Initialize cache if first use */
    if (a_cache == NULL) {
        ensure_a_cache_size(1);
        a_cache[0] = 1;
        a_size = 1;
    }

    ensure_a_cache_size(n);

    for (idx_t i = a_size + 1; i <= n; ++i) {
        /* Ensure primes up to i are available */
        ensure_primes((uint64_t)i);

        seq_t prev = a_cache[i - 2]; /* a(i-1) */
        seq_t sum = 0;

        for (size_t idx = 0; idx < primes_cnt; ++idx) {
            uint64_t p = primes[idx];
            if (p > (uint64_t)i)
                break;
            if ((prev % (seq_t)p) != 0)
                sum += (seq_t)p;
        }

        a_cache[i - 1] = prev + sum;
    }

    a_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not part of the sequence, return 0 */
        return 0;
    compute_a_up_to(n);
    return a_cache[n - 1];
}
