/*
 * A221880  Number of order-preserving or order-reversing full contraction
 *          mappings (of an n-chain) with exactly 1 fixed point.
 * https://oeis.org/A221880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -n + (2^(n-1)*(21*n+34)+8*(-1)^n)/36 (recurrence version)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the OEIS sequence A221880 using the linear recurrence derived from the generating function. */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* Base values of the sequence (indices 1 to 6) */
static const seq_t BASE[6] = { (seq_t)1, (seq_t)2, (seq_t)8, (seq_t)22, (seq_t)57, (seq_t)136 };

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n <= 6) {
        return BASE[n - 1];
    }

    /* Static cache for memoization */
    static seq_t *cache   = NULL;
    static idx_t cache_size   = 0;   /* number of values currently stored */
    static idx_t cache_capacity = 0; /* allocated array size */

    /* Initialise cache if this is the first call */
    if (cache == NULL) {
        /* Start with a moderate capacity; it will grow as required */
        cache_capacity = 64;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) {
            return 0; /* unlikely, but keep the program well‑defined */
        }
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = BASE[i];
        }
        cache_size = 6;
    }

    /* Grow the cache if we need more space */
    if (n > cache_capacity) {
        idx_t new_capacity = cache_capacity;
        while (new_capacity < n) {
            new_capacity *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) {
            return 0; /* allocation failure – rare in practice */
        }
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i represents index i+1 of the sequence (0‑based array) */
        /* Recurrence: a(i) = 5*a(i-1) - 7*a(i-2) - a(i-3) + 8*a(i-4) - 4*a(i-5) */
#ifdef __cplusplus
        if constexpr (std::is_integral_v<seq_t>) {
            /* Use signed __int128 to avoid overflow on 64‑bit builds */
            typedef signed __int128 int128;
            int128 t1 = (int128)cache[i - 1] * 5;
            int128 t2 = (int128)cache[i - 2] * 7;
            int128 t3 = (int128)cache[i - 3];
            int128 t4 = (int128)cache[i - 4] * 8;
            int128 t5 = (int128)cache[i - 5] * 4;
            int128 res = t1 - t2 - t3 + t4 - t5;
            cache[i] = (seq_t)res;
        } else {
#endif
            /* For arbitrary‑precision seq_t, rely on its own arithmetic */
            cache[i] = 5 * cache[i - 1] - 7 * cache[i - 2] - cache[i - 3]
                       + 8 * cache[i - 4] - 4 * cache[i - 5];
#ifdef __cplusplus
        }
#endif
    }
    cache_size = n;
    return cache[n - 1];
}
