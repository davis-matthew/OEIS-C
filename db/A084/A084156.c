/*
 * A084156  Binomial transform of sinh(x)*cosh(sqrt(3)*x).
 * https://oeis.org/A084156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 12*a(n-3) + 3*a(n-4), * a(0) = 0,
 *            a(1) = 1, a(2) = 2, a(3) = 13
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Internal dynamic cache for memoisation */
static seq_t *cache = NULL;     /* array of computed terms */
static size_t cap   = 0;        /* allocated capacity */
static size_t size_cache = 0;   /* number of terms stored (must be <= cap) */

/* Ensure that the cache can hold indices up to 'needed-1' */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed) return;
    size_t new_cap = cap ? cap * 2 : 1;
    while (new_cap < needed) new_cap <<= 1;   /* double until sufficient */

    /* Allocate new array */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* Copy existing data */
    if (size_cache > 0) {
#ifdef __cplusplus
        std::copy(cache, cache + size_cache, new_cache);
#else
        memcpy(new_cache, cache, size_cache * sizeof(seq_t));
#endif
    }

    /* Free old array */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cap   = new_cap;
}

/* Return the n-th term of sequence A084156 */
seq_t a(idx_t n)
{
    /* First four terms are hard‑coded */
    if (n <= 3) {
        static const seq_t base[4] = { (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)13 };
        return base[(size_t)n];
    }

    /* We need to compute up to n, including n */
    size_t nn = (size_t)n;          /* index fits into size_t for N <= 10000 */
    size_t target = nn + 1;         /* number of terms needed (0..n inclusive) */

    /* Allocate or expand cache as necessary */
    ensure_capacity(target);

    /* Compute missing terms iteratively */
    for (size_t i = size_cache; i < target; ++i) {
        seq_t val;
        if (i == 0)   val = (seq_t)0;
        else if (i == 1) val = (seq_t)1;
        else if (i == 2) val = (seq_t)2;
        else if (i == 3) val = (seq_t)13;
        else {
            /* Recurrence:
               a(i) = 4*a(i-1) + 2*a(i-2) - 12*a(i-3) + 3*a(i-4) */
            val = cache[i-1];
            val = val * (seq_t)4U;
            val = val + cache[i-2] * (seq_t)2U;
            val = val + cache[i-4] * (seq_t)3U;
            val = val - cache[i-3] * (seq_t)12U;
        }
        cache[i] = val;
    }

    size_cache = target;
    return cache[nn];
}
