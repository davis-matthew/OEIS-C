/*
 * A021451  Decimal expansion of 1/447.
 * https://oeis.org/A021451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A021451 – decimal expansion of 1/447
 *
 *  a(n) = the n-th digit after the decimal point in 1/447
 *  (OEIS offset 0).  The term is computed by ordinary long
 *  division with a cache so that calls in arbitrary order are
 *  efficient.
 */

#include <stddef.h>     // size_t
#include <stdint.h>     // uint64_t
#include <stdlib.h>     // malloc, free, realloc
#include <string.h>     // memcpy (optional)

#ifdef __cplusplus
#   include <cstddef>   // ensure size_t defined
#endif

/* Helper to grow the cached array */
static void ensure_capacity(idx_t needed) {
    static seq_t *cache = NULL;
    static idx_t cached   = 0;   /* how many digits already stored */
    static idx_t capacity = 0;

    /* No operation in the falling back path; the actual
       allocation is performed within a() below.  */
    (void)cache;
    (void)cached;
    (void)capacity;
#ifndef __cplusplus
    (void)ensure_capacity;
#endif
}

/*[ The following static state is part of the function a() below. ]*/

static seq_t *cache      = NULL;
static idx_t cached      = 0;              /* number of digits stored */
static idx_t capacity    = 0;              /* allocated capacity */
static uint64_t rem      = 1;              /* remainder in long division */
static const uint64_t divisor = 447;       /* denominator */

static void grow_to(idx_t needed) {
    if (needed <= capacity)
        return;

    idx_t new_capacity = capacity > 0 ? capacity * 2 : needed;
    if (new_capacity < needed)
        new_capacity = needed;

    seq_t *new_cache = NULL;

#ifdef __cplusplus
    new_cache = new seq_t[new_capacity];
#else
    new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
#endif

    if (cache != NULL) {
        /* copy existing digits */
        for (idx_t i = 0; i < cached; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }

    cache     = new_cache;
    capacity  = new_capacity;
}

seq_t a(idx_t n) {
    /* Compute digits up to n and cache them */
    if (n + 1 > cached) {
        grow_to(n + 1);
        for (idx_t i = cached; i <= n; ++i) {
            rem *= 10ULL;
            seq_t d = (seq_t)(rem / divisor);
            rem     = rem % divisor;
            cache[i] = d;
        }
        cached = n + 1;
    }

    return cache[n];
}
