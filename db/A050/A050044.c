/*
 * A050044  a(n) = a(n-1) + a(m) for n >= 4, where m = 2*n - 3 - 2^(p+1) and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = a(3) = 2.
 * https://oeis.org/A050044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = a(3) = 2 a(n) = a(n-1) + a(m) for n ≥ 4, where
 *            m = 2*n - 3 - 2^(p+1) and p is the unique integer with 2^p < n-1
 *            ≤ 2^(p+1)
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 195, u64 n <= 770,
 *            u128 n <= 3633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static seq_t *cache = NULL;        /* array holding computed values */
static size_t cache_cap = 0;      /* allocated indices 0..cache_cap inclusive */
static idx_t   cache_max = 0;      /* maximum index computed so far */

/* Ensure that cache has room for indices up to 'needed' (inclusive). */
static void
ensure_cache(size_t needed)
{
    if (needed < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap <= needed) {
        /* protect against overflow of new_cap */
        if (new_cap > SIZE_MAX / 2) {
            new_cap = needed;
            break;
        }
        new_cap <<= 1;
    }

#ifdef __cplusplus
    /* Use C++ new/delete so that big‑int constructors run. */
    seq_t *new_cache = new seq_t[new_cap + 1];
    if (cache) {
        /* copy existing data */
        for (size_t i = 0; i <= cache_cap; ++i) new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
#else
    /* For plain integral seq_t, plain reallocation suffices. */
    seq_t *new_cache = (seq_t *)realloc(cache, ((new_cap + 1) * sizeof(seq_t)));
    if (!new_cache) {
        /* allocation failure – abort */
        abort();
    }
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* outside the sequence definition */

    /* Fast return for the literally small arguments */
    if (n <= 3) {
        return (n == 1) ? (seq_t)1 : (seq_t)2;
    }

    /* Make sure our cache can hold what we need. */
    if ((size_t)n > cache_cap) {
        ensure_cache((size_t)n);
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2 || i == 3) {
            cache[i] = (seq_t)2;
        } else {
            /* p = floor(log2(i - 2))  */
            idx_t n_minus_2 = i - 2ULL;                /* i ≥ 4 → ≥2 */
            /* builtin_clzll is defined for unsigned long long */
            idx_t p = 63 - __builtin_clzll((unsigned long long)n_minus_2);

            idx_t pow2 = ((idx_t)1) << (p + 1);         /* 2^(p+1) */
            idx_t m = (2 * i) - 3 - pow2;              /* m ≥ 1 */

            cache[i] = cache[i - 1] + cache[m];
        }
    }
    cache_max = n;
    return cache[n];
}
