/*
 * A050068  a(n) = a(n-1) + a(m) for n >= 3, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = 3.
 * https://oeis.org/A050068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(2n-3-2^(p+1)) for n≥3, where p is the unique
 *            integer with 2^p < n-1 ≤ 2^(p+1). Initial values: a(1)=1, a(2)=3
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 193, u64 n <= 768,
 *            u128 n <= 3618, big any n
 * Validated: standard, pass, 59 terms through n = 7359 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;          /* array of computed a(n) values   */
static size_t cache_capacity = 0;    /* number of allocated slots       */
static idx_t cache_filled = 0;       /* highest index that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence defined only for n≥1 */

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;               /* indices 0..n */
        seq_t *tmp = seq_realloc(cache, cache_capacity, new_cap);
        if (!tmp) abort();                           /* allocation failure */
        /* Initialise newly allocated elements to zero */
        for (size_t i = cache_capacity; i < new_cap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Initialise base cases once */
    if (cache_filled == 0) {
        cache[1] = (seq_t)1;
        cache_filled = 1;
    }
    if (cache_filled < 2 && n >= 2) {
        cache[2] = (seq_t)3;
        cache_filled = 2;
    }

    /* Compute values up to n if necessary */
    if (n > cache_filled) {
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            /* p is floor(log2(i-2)) because 2^p < i-1 ≤ 2^(p+1) */
            idx_t p   = ilog2(i - 2);
            idx_t pow = ((idx_t)1) << (p + 1);          /* 2^(p+1)   */
            idx_t m   = 2 * i - 3 - pow;               /* m = 2i-3-2^(p+1) */
            cache[i] = cache[i - 1] + cache[m];
        }
        cache_filled = n;
    }

    return cache[n];
}
