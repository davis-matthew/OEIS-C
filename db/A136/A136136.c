/*
 * A136136  a(n+1)=sopfr(2a(n)+1), with sopfr=A001414. Finishes with 10 (fixed
 *          point).
 * https://oeis.org/A136136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n+1) = sopfr(2*a(n)+1), a(1) = 1 (OEIS A136136: fixed point
 *            10)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

/* Count the sum of prime factors (with multiplicity) of a 64‑bit integer.
   Called sopfr in OEIS terminology. */
static seq_t
sopfr_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;                     /* sopfr(1) = 0 */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    return sum;
}

/* Cached values of the sequence.
   Index 0 is unused; indices start at 1 to match the OEIS offset. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* number of elements stored (≥ 1) */

/* Ensure the cache can store a value for any index up to `need`. */
static void
ensure_cache(idx_t need)
{
    if (need + 1 <= cache_cap)
        return;

    size_t new_cap = need + 1;            /* include index 0 */

    seq_t *new_cache = NULL;
#ifdef __cplusplus
    /* In C++ we use operator new[] so that constructors for non‑trivial
       seq_t are invoked. */
    new_cache = new seq_t[new_cap];
#else
    /* In C a simple allocation and zeroing is sufficient. */
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    /* If malloc fails the program is in an undefined state – propagate
       the failure silently, the caller will crash if it attempts to use
       the cache. */
    if (new_cache != NULL)
        memset(new_cache, 0, new_cap * sizeof(seq_t));
#endif

    if (new_cache == NULL)               /* allocation failure – leave old state */
        return;

    /* Copy existing elements to the new cache. */
    if (cache != NULL) {
#ifdef __cplusplus
        /* For C++ copy-construct the old elements. */
        for (size_t i = 1; i < cache_cap && i < new_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
#endif
    }

    cache       = new_cache;
    cache_cap   = new_cap;
}

static inline seq_t
a_recursive(idx_t n)
{
    if (n == 1) {
        cache[1] = 1;
        return 1;
    }

    seq_t prev = a_recursive(n - 1);
    uint64_t x = (uint64_t)prev;               /* fits in 64 bits for this seq. */
    seq_t cur  = sopfr_uint64(2 * x + 1);
    cache[n]   = cur;
    return cur;
}

seq_t
a(idx_t n)
{
    if (n == 0)          /* undefined – return 0 for safety */
        return 0;

    ensure_cache(n);

    /* Cache hit: a(n) already computed. */
    if (cache[n] != 0)
        return cache[n];

    /* First time computing a(n).   */
    return a_recursive(n);
}
