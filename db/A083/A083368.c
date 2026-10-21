/*
 * A083368  a(n) is the position of the highest one in A003754(n).
 * https://oeis.org/A083368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = highest bit‑position that changes when moving from the
 *            fibbinary representation of n-1 to that of n. Defined
 *            recursively: a(1) = 1; if n = ceil(phi * k) (phi = (1+sqrt5)/2)
 *            then a(n) = a(k) + 1 otherwise a(n) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------------- */
/* Utility macros for allocating arrays of seq_t that work in C and C++.      */
/* -------------------------------------------------------------------------- */
#ifdef __cplusplus
#  include <new>          /* std::nothrow */
#  define ALLOC_SEQ_T_ARRAY(sz)     new (std::nothrow) seq_t[(sz)]
#  define FREE_SEQ_T_ARRAY(p)       delete [] (p)
#else
#  include <stdlib.h>      /* malloc, free */
#  define ALLOC_SEQ_T_ARRAY(sz)     ((seq_t*)malloc((sz) * sizeof(seq_t)))
#  define FREE_SEQ_T_ARRAY(p)       free(p)
#endif

#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------------- */
/* Integer square root for values that fit in 128 bits.                       */
/* -------------------------------------------------------------------------- */
static uint64_t isqrt_uint128(__int128 v) {
    __int128 low = 0;
    __int128 high = (__int128)1 << 63;          /* safe upper bound      */
    while (low < high) {
        __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return (uint64_t)low;
}

/* -------------------------------------------------------------------------- */
/* floor( phi * k ), where phi = (1+sqrt(5))/2.                                */
/* -------------------------------------------------------------------------- */
static idx_t floor_phi(idx_t k) {
    /* Compute floor(k * sqrt(5)) = floor(sqrt(5*k^2)) */
    __int128 prod = (__int128)5 * (__int128)k * (__int128)k;
    uint64_t sqrt5k = isqrt_uint128(prod);
    return ((idx_t)k + (idx_t)sqrt5k) / 2U;
}

/* -------------------------------------------------------------------------- */
/* Cache of already computed a(n) values.                                      */
/* -------------------------------------------------------------------------- */
static seq_t *cache = NULL;   /* array index == n; cache[0] unused */
static idx_t cache_size = 0;  /* maximum index currently stored */

/* -------------------------------------------------------------------------- */
/* Ensure that a(i) is known for all 1 <= i <= n.  Allocate or extend cache.  */
/* -------------------------------------------------------------------------- */
static void ensure_cache(idx_t n) {
    if (n <= cache_size)        /* nothing to do                 */
        return;

    /* --- allocate or grow the cache -------------------------------------- */
    idx_t new_size = n + 1;      /* include index 0 which is unused */
    seq_t *new_cache = NULL;

    if (cache == NULL) {
        /* first time: allocate and initialise all entries to 1               */
        new_cache = ALLOC_SEQ_T_ARRAY((size_t)new_size);
        if (!new_cache)          /* allocation failed */
            exit(1);
        for (idx_t i = 1; i < new_size; ++i)
            new_cache[i] = (seq_t)1;   /* default value for all n            */
        new_cache[1] = (seq_t)1;        /* a(1) = 1 more explicit              */
        cache = new_cache;
        cache_size = n;
    } else {
        /* grow existing cache to new_size */
        seq_t *tmp = ALLOC_SEQ_T_ARRAY((size_t)new_size);
        if (!tmp) exit(1);

        /* copy old values */
        for (idx_t i = 1; i <= cache_size; ++i)
            tmp[i] = cache[i];

        /* initialise new entries to 1 */
        for (idx_t i = cache_size + 1; i < new_size; ++i)
            tmp[i] = (seq_t)1;

        FREE_SEQ_T_ARRAY(cache);
        cache = tmp;
        cache_size = n;
    }

    /* --- recompute positions of the form ceil(phi*k) -------------------- */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t target = floor_phi(k) + 1U;          /* ceil(phi*k) */
        if (target <= n)
            cache[target] = cache[k] + (seq_t)1;
    }
}

/* -------------------------------------------------------------------------- */
/* Public function implementing the sequence.                                */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0)               /* below offset – undefined */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
