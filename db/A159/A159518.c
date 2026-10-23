/*
 * A159518  Numerator of Hermite(n, 11/15).
 * https://oeis.org/A159518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 22*a(n-1) - 450*(n-1)*a(n-2),
 * a(0) = 1, a(1) = 22.
 *
 * This implements O(n) time memoisation.  The function is
 * validate in both C11 and C++17.  In C mode an array
 * allocated with malloc is used; in C++ a std::vector
 * is used so that arbitrary‑precision seq_t types are
 * properly constructed.
 */

#ifndef _SEQ_A159518_H_
#define _SEQ_A159518_H_

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>   /* for size_t */

/* The helper types seq_t and idx_t are supplied by the build. */
extern seq_t a(idx_t n);

#ifdef __cplusplus
/* C++ implementation ----------------------------------------------------- */

static std::vector<seq_t> cache;   /* memoised values */
static idx_t            last_idx = 0U;   /* largest index computed */

/* initialise cache with the first two terms */
static void
ensure_initialisation()
{
    if (cache.empty()) {
        cache.resize(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        last_idx = 1U;
    }
}

static void
ensure_capacity(idx_t n)
{
    /* ensure we can index a[n] */
    if (cache.size() <= n) {
        size_t new_cap = cache.size() * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        cache.resize(new_cap);
    }
}

seq_t
a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    ensure_initialisation();
    if (n > last_idx) {
        ensure_capacity(n);
        for (idx_t i = last_idx + 1U; i <= n; ++i) {
            if (i == 1U) {
                cache[1] = (seq_t)22;
            } else {
                /* a(i) = 22*a(i-1) - 450*(i-1)*a(i-2) */
                cache[i] =
                    (seq_t)22 * cache[i - 1] -
                    (seq_t)450 * ((seq_t)(i - 1U)) * cache[i - 2];
            }
        }
        last_idx = n;
    }
    return cache[n];
}

#else   /* ---------- C implementation ------------------------------------- */

#include <stdlib.h>   /* for malloc, realloc, free */

/* memoised cache for C implementation */
static seq_t *cache      = NULL;   /* sequence values */
static idx_t  capacity   = 0U;      /* allocated size */
static idx_t  last_idx   = 0U;      /* largest index computed */

/* initialise the cache with the first two terms */
static void
ensure_initialisation(void)
{
    if (cache == NULL) {
        capacity = 2U;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        last_idx = 1U;
    }
}

/* grow the cache array to hold index n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if (capacity > n)
        return;
    while (capacity <= n) {
        capacity = (capacity == 0U) ? 2U : capacity * 2U;
    }
    cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
}

/* free the cache when the program terminates */
static void
free_cache(void)
{
    free(cache);
    cache = NULL;
    capacity = 0U;
    last_idx = 0U;
}

seq_t
a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;

    ensure_initialisation();
    if (n > last_idx) {
        ensure_capacity(n);
        for (idx_t i = last_idx + 1U; i <= n; ++i) {
            if (i == 1U) {
                cache[1] = (seq_t)22;
            } else {
                /* a(i) = 22*a(i-1) - 450*(i-1)*a(i-2) */
                cache[i] =
                    (seq_t)22 * cache[i - 1] -
                    (seq_t)450 * ((seq_t)(i - 1U)) * cache[i - 2];
            }
        }
        last_idx = n;
    }
    return cache[n];
}

/* register the cleanup function */
static void __attribute__((constructor)) init(void)
{
    /* nothing needed here; cache will be allocated on first use */
}
static void __attribute__((destructor)) cleanup(void)
{
    free_cache();
}

#endif  /* end C / C++ branch */

#endif  /* _SEQ_A159518_H_ */
