/*
 * A130292  Numbers that are sums of fifth powers of two distinct primes.
 * https://oeis.org/A130292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th smallest distinct sum of two distinct primes to the
 *            fifth power
 * Range:     u8 none, u16 n <= 3, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

/* required number of distinct primes so that (p choose 2) >= n */
static size_t required_prime_count(idx_t n)
{
    /* Solve p*(p-1)/2 >= n -> p ≈ ceil((1+√(1+8n))/2) */
    double dn = (double)n;
    double root = sqrt(1.0 + 8.0 * dn);
    size_t p = (size_t)((1.0 + root) * 0.5);
    if ((seq_t)p * (seq_t)(p - 1) / 2 < n) ++p;
    return p;
}

/* comparison for qsort on seq_t */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* static storage for the produced sums */
static seq_t *cache = NULL;   /* sorted unique sums */
static size_t cached_len = 0; /* number of values stored */

/* enlarge cache so that it contains at least n values */
static void grow_cache(idx_t n)
{
    if (cached_len >= (size_t)n) return;        /* already enough */

    size_t prime_needed = required_prime_count(n);
    uint64_t limit;

    /* Estimate an upper bound for the nth prime */
    if (prime_needed < 6) {
        limit = 15;
    } else {
        double dm = (double)prime_needed;
        double est = dm * (log(dm) + log(log(dm)));
        limit = (uint64_t)(est) + 10;
    }

    /* Ask for primes until we have enough */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    while (1) {
        build_primes_upto((idx_t)limit, &primes, &primes_cnt);
        if (primes_cnt >= prime_needed) break;
        free(primes);
        limit *= 2;
    }

    /* compute p^5 for each prime */
    size_t m = primes_cnt;
    seq_t *p5 = mem_new(seq_t, m);
    for (size_t i = 0; i < m; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t t = p;
        t *= p; /* p^2 */
        t *= p; /* p^3 */
        t *= p; /* p^4 */
        t *= p; /* p^5 */
        p5[i] = t;
    }

    /* generate all unordered distinct sums of two fifth powers */
    size_t pair_cnt = m * (m - 1) / 2;
    seq_t *sums = mem_new(seq_t, pair_cnt);
    size_t idx = 0;
    for (size_t i = 0; i < m - 1; ++i) {
        seq_t pi5 = p5[i];
        for (size_t j = i + 1; j < m; ++j) {
            sums[idx++] = pi5 + p5[j];
        }
    }

    /* sort the sums */
    mem_qsort(sums, pair_cnt, sizeof(seq_t), cmp_seq);

    /* deduplicate to obtain the sequence terms */
    size_t new_len = 0;
    if (pair_cnt > 0) {
        new_len = 1;
        for (size_t i = 1; i < pair_cnt; ++i)
            if (cmp_seq(&sums[i], &sums[i - 1]) != 0)
                ++new_len;
    }

    seq_t *new_cache = mem_new(seq_t, new_len);
    if (pair_cnt > 0) {
        new_cache[0] = sums[0];
        size_t dst = 1;
        for (size_t i = 1; i < pair_cnt; ++i)
            if (cmp_seq(&sums[i], &sums[i - 1]) != 0)
                new_cache[dst++] = sums[i];
    }

    /* free temporary data */
    mem_free(cache);
    mem_free(sums);
    mem_free(p5);
    free(primes);

    /* store the new cache */
    cache = new_cache;
    cached_len = new_len;
}

/* public function required by the testing harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but be safe */
    grow_cache(n);
    return cache[(size_t)n - 1];
}
