/*
 * A104805  "Ceiling of hypotenuses": a(n) = ceiling(sqrt(a(n-1)^2 +
 *          a(n-2)^2)), a(1)=1, a(2)=3.
 * https://oeis.org/A104805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(a(n-1)^2 + a(n-2)^2)), a(1)=1, a(2)=3
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   ceil_sqrt_uint128, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/isqrt.h"
#include "../utils/ceil_sqrt_uint128.h"

#ifdef __cplusplus
/* C++ build: sequence type is an arbitrary‑precision class.
   Use the generic integer operators and the provided isqrt. */
#else

#endif

/* Cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;   /* allocated size in terms of indices */
static idx_t max_computed = 0;     /* highest index stored in cache */

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_capacity < n + (idx_t)1) {
        cache = (seq_t *)realloc(cache, (size_t)(n + (idx_t)1) * sizeof(seq_t));
        cache_capacity = n + (idx_t)1;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;               /* undefined for n<1, return (safe) */

    /* Base cases */
    if (n == (idx_t)1)
        return (seq_t)1;
    if (n == (idx_t)2)
        return (seq_t)3;

    /* Expand cache if necessary */
    ensure_cache(n);

    /* Initialise first two entries if missing */
    if (max_computed < (idx_t)1) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }
    if (max_computed < (idx_t)2) {
        cache[2] = (seq_t)3;
        max_computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + (idx_t)1; i <= n; ++i) {
#ifdef __cplusplus
        seq_t x = cache[i-1];
        seq_t y = cache[i-2];
        seq_t sum = x * x + y * y;      /* arbitrary‑precision arithmetic */
        seq_t root = isqrt(sum);        /* floor sqrt */
        if (root * root != sum)
            root = root + (seq_t)1;     /* ceil */
        cache[i] = root;
#else
        seq_t x = cache[i-1];
        seq_t y = cache[i-2];
        unsigned __int128 xx = (unsigned __int128)x;
        unsigned __int128 yy = (unsigned __int128)y;
        unsigned __int128 sum128 = xx * xx + yy * yy;
        unsigned __int128 root128 = ceil_sqrt_uint128(sum128);
        cache[i] = (seq_t)root128;
#endif
    }

    max_computed = n;
    return cache[n];
}
