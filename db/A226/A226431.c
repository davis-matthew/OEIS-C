/*
 * A226431  The number of permutations of length n in a particular geometric
 *          grid class.
 * https://oeis.org/A226431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1) - 31*a(n-2) + 51*a(n-3) - 41*a(n-4) 
   + 15*a(n-5) - 2*a(n-6) with a(1)=1, a(2)=2, a(3)=6, a(4)=21, 
   a(5)=73, a(6)=244. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /*    cache[0] is unused; cache[i] holds a(i) for i >= 1          */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;   /* last index that has a value   */

    if (cache == NULL) {
        cache_size = (n + 1 > 7 ? n + 1 : 7);
        cache = seq_alloc_new_array(cache_size);
    }
    else if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = seq_alloc_new_array(new_size);
        for (idx_t i = 0; i <= computed_max && i < new_size; ++i)
            tmp[i] = cache[i];
        /* the old block may be leaked – acceptable for this use case */
        cache = tmp;
        cache_size = new_size;
    }

    if (n < 1) return (seq_t)0;
    if (n <= computed_max) return cache[n];

    /* initialise the first six values once */
    if (computed_max < 6) {
        if (cache_size > 1) cache[1] = (seq_t)1;
        if (cache_size > 2) cache[2] = (seq_t)2;
        if (cache_size > 3) cache[3] = (seq_t)6;
        if (cache_size > 4) cache[4] = (seq_t)21;
        if (cache_size > 5) cache[5] = (seq_t)73;
        if (cache_size > 6) cache[6] = (seq_t)244;
        computed_max = 6;
    }

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t v = mul_small(cache[i-1], 9);
        seq_t t2 = mul_small(cache[i-2], 31);
        v += (~t2) + 1;
        seq_t t3 = mul_small(cache[i-3], 51);
        v += t3;
        seq_t t4 = mul_small(cache[i-4], 41);
        v += (~t4) + 1;
        seq_t t5 = mul_small(cache[i-5], 15);
        v += t5;
        seq_t t6 = mul_small(cache[i-6], 2);
        v += (~t6) + 1;
        cache[i] = v;
    }
    computed_max = n;
    return cache[n];
}
