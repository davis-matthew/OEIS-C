/*
 * A087797  Primes, squares of primes and cubes of primes.
 * https://oeis.org/A087797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the union of the primes, the squares
 *            of primes, and the cubes of primes, in increasing order,
 *            duplicates removed
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Comparator for qsort that works for any supported seq_t. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Static cache data. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t sieve_limit = 0;

/* Build cache of the sequence up to the given limit. */
static void build_cache(idx_t limit)
{
    /* 1. Sieve to find primes <= limit. */
    size_t n = (size_t)limit + 1;
    bool *mark = (bool *)malloc(n * sizeof(bool));
    if (!mark) exit(EXIT_FAILURE);
    for (size_t i = 0; i < n; ++i) mark[i] = true;
    if (n > 0) mark[0] = false;
    if (n > 1) mark[1] = false;
    for (size_t i = 2; i * i < n; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j < n; j += i) mark[j] = false;
        }
    }

    /* 2. Collect primes into array. */
    size_t prime_cnt = 0;
    for (size_t i = 2; i < n; ++i) if (mark[i]) prime_cnt++;
    uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t pidx = 0;
    for (size_t i = 2; i < n; ++i) if (mark[i]) primes[pidx++] = (uint64_t)i;
    free(mark);

    /* 3. Generate candidates (primes, squares, cubes). */
    size_t cand_cap = prime_cnt * 3;
    seq_t *cand = (seq_t *)malloc(cand_cap * sizeof(seq_t));
    if (!cand) exit(EXIT_FAILURE);
    size_t cand_len = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        cand[cand_len++] = (seq_t)p;
        if ((idx_t)p * (idx_t)p <= limit) cand[cand_len++] = (seq_t)p * (seq_t)p;
        if ((idx_t)p * (idx_t)p * (idx_t)p <= limit)
            cand[cand_len++] = (seq_t)p * (seq_t)p * (seq_t)p;
    }
    free(primes);

    /* 4. Sort and deduplicate. */
    qsort(cand, cand_len, sizeof(seq_t), cmp_seq_t);
    seq_t *new_cache = (seq_t *)malloc(cand_len * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    size_t new_len = 0;
    if (cand_len > 0) {
        new_cache[new_len++] = cand[0];
        for (size_t i = 1; i < cand_len; ++i)
            if (cand[i] != cand[i-1]) new_cache[new_len++] = cand[i];
    }
    free(cand);

    /* 5. Replace old cache. */
    if (cache) free(cache);
    cache = new_cache;
    cache_len = new_len;
    sieve_limit = limit;
}

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;
    if (sieve_limit == 0) build_cache(10);
    while (cache_len < (size_t)n) {
        idx_t new_l = sieve_limit * 2;
        if (new_l <= sieve_limit) new_l = sieve_limit + 1;
        build_cache(new_l);
    }
}

/* -------------------------------------------------------------------------- */
/* Public sequence function: returns the n‑th term (1‑based indexing).         */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    ensure_cache(n);
    return cache[n - 1];
}
