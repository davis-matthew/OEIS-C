/*
 * A087787  a(n) = Sum_{k=0..n} (-1)^(n-k)*A000041(k).
 * https://oeis.org/A087787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) - a(n-1) Initial values: a(0)=1, p(0)=1 Here p(k) is
 *            the partition number of k. The function keeps two caches
 *            (p_cache and a_cache) that grow on demand. The implementation
 *            uses the pentagonal‑number theorem to compute p(k) in O(k^1/2)
 *            per term. All arithmetic is done with the seq_t ty...
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 134, u64 n <= 427,
 *            u128 n <= 1478, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

/* C++ path: use std::vector, C path: use raw pointers */
#ifdef __cplusplus
static std::vector<seq_t> p_cache;
static std::vector<seq_t> a_cache;
static idx_t p_last = 0;
static idx_t a_last = 0;
#else
static seq_t *p_cache = NULL;
static seq_t *a_cache = NULL;
static idx_t p_last = 0;
static idx_t a_last = 0;
static size_t p_cap = 0;
static size_t a_cap = 0;
#endif

/* Ensure p_cache has at least n+1 entries (C only) */
#ifndef __cplusplus
static void ensure_p_cache(idx_t n)
{
    if (p_cap <= (size_t)n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = p_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        p_cache = tmp;
        p_cap   = new_cap;
    }
}

/* Ensure a_cache has at least n+1 entries (C only) */
static void ensure_a_cache(idx_t n)
{
    if (a_cap <= (size_t)n) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = a_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        a_cache = tmp;
        a_cap   = new_cap;
    }
}
#endif

/* Compute partition numbers up to n */
static void compute_partitions(idx_t n)
{
    /* p(0) must already be initialized */
    for (idx_t m = p_last + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1) sum += p_cache[m - g1];
            else       sum -= p_cache[m - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            if (k & 1) sum += p_cache[m - g2];
            else       sum -= p_cache[m - g2];
        }
        p_cache[m] = sum;
    }
    p_last = n;
}

/* Compute a(n) = p(n) - a(n-1) up to n */
static void compute_a(idx_t n)
{
    for (idx_t m = a_last + 1; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = p_cache[0];        /* p(0) = 1 */
        } else {
            a_cache[m] = p_cache[m] - a_cache[m - 1];
        }
    }
    a_last = n;
}

/* Main entry point */
seq_t a(idx_t n)
{
    /* Lazy initialization */
#ifndef __cplusplus
    if (p_cache == NULL) {
        p_cache = (seq_t *)calloc(1, sizeof(seq_t));
        a_cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!p_cache || !a_cache) abort();

        /* Base values */
        p_cache[0] = (seq_t)1;  /* partition of 0 */
        a_cache[0] = (seq_t)1;  /* a(0) = 1 */

        p_cap = a_cap = 1;
        p_last = a_last = 0;
    }
#else
    if (p_cache.empty()) {
        p_cache.reserve(1);
        p_cache.push_back((seq_t)1);   /* p(0)=1 */
        a_cache.reserve(1);
        a_cache.push_back((seq_t)1);   /* a(0)=1 */
        p_last = a_last = 0;
    }
#endif

    /* Ensure caches large enough */
#ifndef __cplusplus
    if (n > p_last) {
        ensure_p_cache(n);
        compute_partitions(n);
    }
    if (n > a_last) {
        ensure_a_cache(n);
        compute_a(n);
    }
#else
    if (n > p_last) {
        p_cache.resize((size_t)(n + 1));
        compute_partitions(n);
    }
    if (n > a_last) {
        a_cache.resize((size_t)(n + 1));
        compute_a(n);
    }
#endif

    return a_cache[n];
}
