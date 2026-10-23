/*
 * A120398  Sums of two distinct prime cubes.
 * https://oeis.org/A120398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth smallest sum of two distinct prime cubes. (Computed
 *            by precomputing all sums of prime cubes with primes ≤ 5000,
 *            sorting, and removing duplicates.)
 * Range:     u8 none, u16 none, u32 n <= 1950, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* compare function for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Persistent cache for the sequence */
static seq_t *seq_cache   = NULL;   /* sorted, unique sums */
static size_t seq_len     = 0;      /* length of seq_cache */
static bool   initialized = false;

/* Build the cache on first call */
static void build_cache(void)
{
    /* Prime limit sufficient to generate > 10000 terms */
    const idx_t PRIME_LIMIT = 5000;
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;

    /* Generate all primes up to PRIME_LIMIT */
    build_primes_upto(PRIME_LIMIT, &primes, &prime_cnt);

    /* Pre‑compute cubes of each prime */
    seq_t *cubes = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        cubes[i] = (seq_t)primes[i] * (seq_t)primes[i] * (seq_t)primes[i];

    /* Estimate number of sums: odd (2000^3) + even pairs */
    size_t max_sums = prime_cnt * (prime_cnt - 1) / 2 + (prime_cnt - 1);
    seq_t *sums = (seq_t *)malloc(max_sums * sizeof(seq_t));
    size_t len = 0;

    /* Add odd sums 8 + p^3 for all primes p > 2 */
    for (size_t i = 0; i < prime_cnt; ++i)
        if (primes[i] > 2)
            sums[len++] = (seq_t)8 + cubes[i];

    /* Add even sums p^3 + q^3 with distinct primes > 2 */
    /* Skip prime 2 to avoid duplicates with odd sums */
    size_t start = 0;
    while (start < prime_cnt && primes[start] == 2) ++start;
    for (size_t i = start; i < prime_cnt; ++i) {
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            sums[len++] = cubes[i] + cubes[j];
        }
    }

    /* Sort the sums */
    qsort(sums, len, sizeof(seq_t), cmp_seq);

    /* Remove duplicates */
    seq_len = 0;
    for (size_t i = 0; i < len; ++i) {
        if (seq_len == 0 || sums[i] != sums[seq_len - 1])
            sums[seq_len++] = sums[i];
    }

    /* Allocate cache of exact size */
    seq_cache = (seq_t *)realloc(sums, seq_len * sizeof(seq_t));
    /* cubes and primes arrays are freed automatically when function returns,
       as they were allocated on the heap and are no longer needed. */
    free(cubes);
    free(primes);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        build_cache();

    if ((idx_t)seq_len < n || n == 0)
        return (seq_t)0;   /* undefined for out of bounds; tests only use n>=1 within range */

    return seq_cache[(size_t)(n - 1)];
}
