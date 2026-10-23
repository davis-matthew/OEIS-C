/*
 * A167110  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A167110 – Number of reduced words of length n in Coxeter
 *  group on 9 generators S_i with (S_i)^2 = (S_i S_j)^14 = I.
 *
 *  The sequence satisfies the linear recurrence
 *
 *      a(n) = 7 * (a(n-1)+a(n-2)+...+a(n-13)) - 28 * a(n-14)
 *      for n ≥ 15,
 *
 *  with initial values a(0)=1, a(1)=9, a(2)=72, a(3)=576, a(4)=4608,
 *  a(5)=36864, a(6)=294912, a(7)=2359296, a(8)=18874368,
 *  a(9)=150994944, a(10)=1207959552, a(11)=9663676416,
 *  a(12)=77309411328, a(13)=618475290624, a(14)=4947802324956.
 *
 *  The function uses a static cache to allow calls in arbitrary order.
 */

#include <stdlib.h>  /* for malloc, free, size_t */
#include <stdint.h>  /* for uintptr_t, maybe needed by build */

seq_t a(idx_t n)
{
    /* static cache structure */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of values stored */
    static idx_t cache_capacity = 0;      /* allocated space in terms */

    /* Initial terms up to a(14) */
    static const seq_t init_vals[] = {
        (seq_t)1,          /* a(0) */
        (seq_t)9,          /* a(1) */
        (seq_t)72,         /* a(2) */
        (seq_t)576,        /* a(3) */
        (seq_t)4608,       /* a(4) */
        (seq_t)36864,      /* a(5) */
        (seq_t)294912,     /* a(6) */
        (seq_t)2359296,    /* a(7) */
        (seq_t)18874368,   /* a(8) */
        (seq_t)150994944,  /* a(9) */
        (seq_t)1207959552, /* a(10) */
        (seq_t)9663676416, /* a(11) */
        (seq_t)77309411328,    /* a(12) */
        (seq_t)618475290624,   /* a(13) */
        (seq_t)4947802324956   /* a(14) */
    };
    const idx_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);

    /* Allocate or grow cache if needed */
    if (cache == NULL) {
        cache_capacity = init_len;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init_vals[i];
        cache_size = init_len;
    } else if (n >= cache_capacity) {
        /* grow to min(n+1, 2*cache_capacity) */
        idx_t new_capacity = n + 1;
        if (new_capacity < cache_capacity * 2)
            new_capacity = cache_capacity * 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_capacity);
        if (tmp == NULL)
            return 0; /* allocation failure: return 0 as sentinel */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* If requested n already computed, return it */
    if (n < cache_size)
        return cache[n];

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term;
        if (i < init_len) {
            /* Should not happen because cache_size <= init_len-1 */
            term = init_vals[i];
        } else {
            /* Recurrence for i >= 15 (index 15 corresponds to a(15)) */
            seq_t sum13 = 0;
            for (idx_t k = 1; k <= 13; ++k)
                sum13 += cache[i - k];
            term = sum13 * ((seq_t)7) - cache[i - 14] * ((seq_t)28);
        }
        cache[i] = term;
    }
    cache_size = n + 1;  /* now all terms up to n are cached */

    return cache[n];
}
