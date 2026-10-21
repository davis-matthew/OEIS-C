/*
 * A227318  G.f.: Sum_{n>=0} x^n * (1-x)^A007814(n), where A007814(n) is the
 *          exponent of the highest power of 2 dividing n.
 * https://oeis.org/A227318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Σ_{m≥0} x^m * (1-x)^{v₂(m)} where
 *            v₂(m) is the exponent of the highest power of 2 dividing m
 * Range:     i8 n <= 1027, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Forward declaration of seq_t & idx_t – they are defined in the build. */
#pragma once

/* Helper: trailing‑zero count (v₂ of a non‑negative integer) */
static inline unsigned int v2_idx(idx_t v)
{
    if (v == 0) return 0;
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_ctzll((uint64_t)v);
#else
    unsigned int c = 0;
    while ((v & 1u) == 0) { c++; v >>= 1; }
    return c;
#endif
}

/* Helper: binomial coefficient C(v,k) computed with 128‑bit intermediates */
static inline seq_t binom_small(unsigned int v, unsigned int k)
{
    if (k > v) return 0;
    if (k > v - k) k = v - k;          /* use symmetry */
    __int128 r = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        r = r * (v - k + i) / i;       /* intermediate fits in 128 bits */
    }
    return (seq_t)r;
}

#ifdef __cplusplus
/* C++ implementation – uses std::vector for proper construction of
   arbitrary‑precision types. */
#include <vector>
#include <cstddef>
#include <cstdint>

static std::vector<seq_t> cache_vec;   /* cache of computed values */
static idx_t current_max = (idx_t)(-1);/* last index that has been filled */

/* Ensure cache_vec has size at least ‘min_size’. New elements are zero‑initialized. */
static inline void ensure_capacity(idx_t min_size)
{
    if (cache_vec.size() < min_size) {
        cache_vec.resize(min_size, 0);
    }
}
#else
/* C implementation – uses malloc/realloc on a raw array. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache_arr = NULL;    /* pointer to allocated memory */
static idx_t cache_size = 0;       /* number of allocated elements */
static idx_t current_max = (idx_t)(-1); /* last index that has been processed */

static inline void ensure_capacity(idx_t min_size)
{
    if (cache_arr == NULL) {
        /* initial allocation */
        cache_arr = (seq_t *)malloc(min_size * sizeof(seq_t));
        if (cache_arr == NULL) exit(1);
        memset(cache_arr, 0, min_size * sizeof(seq_t));
        cache_size = min_size;
        return;
    }
    if (cache_size < min_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < min_size) new_size = min_size;
        seq_t *tmp = (seq_t *)realloc(cache_arr, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        /* zero the newly allocated part */
        memset(tmp + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache_arr = tmp;
        cache_size = new_size;
    }
}
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* Expand the cache so that we have indices up to ‘n’ ready to receive
       contributions from future terms. */
    ensure_capacity(n + 1);   /* at least n+1 elements */

    /* Incrementally compute contributions for indices > current_max. */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        unsigned int v = v2_idx(i);
        for (unsigned int k = 0; k <= v; ++k) {
            seq_t coeff = binom_small(v, k);
            if (k & 1) coeff = -coeff;               /* (-1)^k factor */
            idx_t pos = i + k;
            ensure_capacity(pos + 1);
            cache_vec[pos] += coeff;
        }
    }
    current_max = n;
    return cache_vec[n];

#else   /* C */
    /* Ensure we have storage for index n and any future positions that
       may be touched by contributions of indices up to n. */
    idx_t needed = n + 1;          /* for accessing cache[n] */
    ensure_capacity(needed);

    /* Compute new contributions if we haven't already reached n. */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        unsigned int v = v2_idx(i);
        for (unsigned int k = 0; k <= v; ++k) {
            seq_t coeff = binom_small(v, k);
            if (k & 1) coeff = -coeff;               /* (-1)^k factor */
            idx_t pos = i + k;
            ensure_capacity(pos + 1);
            cache_arr[pos] += coeff;
        }
    }
    current_max = n;
    return cache_arr[n];
#endif
}
