/*
 * A208225  a(n)=(a(n-1)^3*a(n-3)^3+a(n-2))/a(n-4) with a(0)=a(1)=a(2)=a(3)=1.
 * https://oeis.org/A208225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (a(n-1)^3 * a(n-3)^3 + a(n-2)) / a(n-4), 
 * a(0)=a(1)=a(2)=a(3)=1
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* for alloc_seq_array() */
#include "../utils/free_cache.h"        /* for free_cache() */

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;            /* array of computed terms */
    static idx_t cache_len  = 0;                /* number of terms initialized (0=uninitialized) */
    static idx_t cache_cap  = 0;                /* current capacity of cache */

    /* base cases ------------------------------------------------------- */
    if (n < 4) {
        return (seq_t)1;
    }

    /* initialize cache if first call ---------------------------------- */
    if (cache == NULL) {
        /* start with capacity 4 to hold indices 0..3 */
        cache_cap = 4;
        cache_len = 4;
        cache     = alloc_seq_array(cache_cap);
        for (idx_t i = 0; i < 4; ++i) {
            cache[i] = (seq_t)1;
        }
    }

    /* grow and fill cache up to required index ------------------------ */
    if (n >= cache_len) {
        /* grow capacity if needed (power‑of‑two growth) */
        if ((idx_t)(n + 1) > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap <= n) {
                new_cap <<= 1;               /* double */
            }
            seq_t *new_cache = alloc_seq_array(new_cap);
            for (idx_t i = 0; i < cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free_cache(cache);
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* compute remaining terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) {                       /* should not happen, but safety */
                cache[i] = (seq_t)1;
            } else {
                seq_t a_nm1 = cache[i - 1];
                seq_t a_nm2 = cache[i - 2];
                seq_t a_nm3 = cache[i - 3];
                seq_t a_nm4 = cache[i - 4];

                seq_t a_nm1_cubed = a_nm1 * a_nm1 * a_nm1;
                seq_t a_nm3_cubed = a_nm3 * a_nm3 * a_nm3;
                seq_t numerator   = a_nm1_cubed * a_nm3_cubed + a_nm2;
                cache[i] = numerator / a_nm4;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
