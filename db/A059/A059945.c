/*
 * A059945  Number of 4-block bicoverings of an n-set.
 * https://oeis.org/A059945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (6^n - 4*3^n - 3*2^n + 12) / 24
 * Recurrence (for n > 4): a(n) = 12*a(n-1) - 47*a(n-2) + 72*a(n-3) - 36*a(n-4)
 * Initial values: a(1)=0, a(2)=0, a(3)=4, a(4)=39
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Static cache for memoisation */
static seq_t *cache = NULL;     /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;    /* allocated size of cache (number of elements) */
static idx_t cached_up_to = 0;  /* maximum n that has been computed (1‑based) */

/* Ensure the cache has space for terms up to n (1‑based) */
static void ensure_capacity(idx_t n)
{
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – leave cache unchanged */
        cache = tmp;
        cache_size = n;
    }
}

/* Compute a single value for index n (n ≥ 5), assuming cache holds previous terms */
static seq_t compute_next(idx_t n)   /* n: 1‑based index */
{
    /* base cases are handled outside this function */
#ifdef __cplusplus
    if (std::is_integral<seq_t>::value) {
        /* seq_t is an integral type: use 128‑bit intermediate */
        __int128 prod1 = (__int128)12 * (__int128)cache[n - 2];
        __int128 prod2 = (__int128)47 * (__int128)cache[n - 3];
        __int128 prod3 = (__int128)72 * (__int128)cache[n - 4];
        __int128 prod4 = (__int128)36 * (__int128)cache[n - 5];
        __int128 r = prod1 - prod2 + prod3 - prod4;
        return (seq_t)r;
    } else {
        /* seq_t is an arbitrary‑precision class */
        seq_t r = 12 * cache[n - 2];
        r -= 47 * cache[n - 3];
        r += 72 * cache[n - 4];
        r -= 36 * cache[n - 5];
        return r;
    }
#else
    /* C build: seq_t is an integral type */
    __int128 prod1 = (__int128)12 * (__int128)cache[n - 2];
    __int128 prod2 = (__int128)47 * (__int128)cache[n - 3];
    __int128 prod3 = (__int128)72 * (__int128)cache[n - 4];
    __int128 prod4 = (__int128)36 * (__int128)cache[n - 5];
    __int128 r = prod1 - prod2 + prod3 - prod4;
    return (seq_t)r;
#endif
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range; return 0 for safety */

    if (n > cached_up_to) {
        ensure_capacity(n);
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            if (i <= 4) {
                /* base cases */
                switch (i) {
                    case 1:
                    case 2: cache[i - 1] = (seq_t)0; break;
                    case 3: cache[i - 1] = (seq_t)4; break;
                    case 4: cache[i - 1] = (seq_t)39; break;
                }
            } else {
                cache[i - 1] = compute_next(i);
            }
        }
        cached_up_to = n;
    }
    return cache[n - 1];
}
