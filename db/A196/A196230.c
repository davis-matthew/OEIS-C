/*
 * A196230  Euler primes: values of x^2 - x + k for x = 1..k-1, where k is one
 *          of Euler's "lucky" numbers 2, 3, 5, 11, 17, 41.
 * https://oeis.org/A196230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A196230:
   primes of the form x^2 - x + k where k ∈ {2,3,5,11,17,41} and
   1 ≤ x < k.  The resulting primes are sorted in ascending order
   and returned by a(n) for n ≥ 1 (n = 1 yields the smallest prime).  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* List of Euler "lucky" numbers k. */
static const unsigned int lucky_k[] = {2, 3, 5, 11, 17, 41};
static const size_t num_k = sizeof(lucky_k) / sizeof(lucky_k[0]);

/* Cache for the sorted primes.  */
static seq_t  cache[73] = {0};
static size_t cache_len  = 0;
static bool   cache_built = false;

/* Simple primality test for small numbers in seq_t.  */
static bool is_prime_small(seq_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;

    for (seq_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

/* Comparator for qsort (ascending).  */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb)
        return -1;
    else if (*va > *vb)
        return 1;
    else
        return 0;
}

/* Build the cached sequence on first call.  */
static void build_cache(void)
{
    seq_t temp[73];
    size_t tlen = 0;

    for (size_t i = 0; i < num_k; ++i) {
        unsigned int k = lucky_k[i];
        for (unsigned int x = 1; x < k; ++x) {
            seq_t val = (seq_t)x * (seq_t)x - (seq_t)x + (seq_t)k;
            if (is_prime_small(val))
                temp[tlen++] = val;
        }
    }

    /* Sort and remove duplicates.  */
    qsort(temp, tlen, sizeof(seq_t), seq_cmp);

    size_t out_idx = 0;
    for (size_t i = 0; i < tlen; ++i) {
        if (out_idx == 0 || temp[i] != cache[out_idx - 1])
            cache[out_idx++] = temp[i];
    }

    cache_len   = out_idx;
    cache_built = true;
}

/* External linkage function required by the problem statement.  */
seq_t a(idx_t n)
{
    if (!cache_built)
        build_cache();

    if (n == 0)
        return 0;  /* Undefined offset; return 0 for safety. */

    if ((idx_t)(n - 1) < cache_len)
        return cache[n - 1];

    return 0;  /* n is out of range – sequence ends. */
}
