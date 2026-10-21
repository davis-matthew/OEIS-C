/*
 * A209381  1/4 the number of (n+1) X 8 0..2 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209381: 1/4 the number of (n+1)×8 0..2 arrays with every 2×2
 * subblock having distinct edge sums.
 *
 * Formula (empirical recurrence, valid for n ≥ 10):
 *   a(n) = 32·a(n‑2) – 308·a(n‑4) + 1072·a(n‑6) – 1152·a(n‑8)
 *
 * Initial values (for 1 ≤ n ≤ 10):
 *   a(1)=256,  a(2)=882,  a(3)=1792,  a(4)=8612,
 *   a(5)=18176, a(6)=114056, a(7)=242176, a(8)=1784720,
 *   a(9)=3777536, a(10)=30197792
 *
 * The function memoizes computed values so that repeated calls in any
 * order are fast.  It runs in O(n) time for the first call to compute
 * up to a given n, and O(1) thereafter.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1. constant initial values */
    static const seq_t init[11] = {
        0,
        256,      /*  a(1)  */
        882,      /*  a(2)  */
        1792,     /*  a(3)  */
        8612,     /*  a(4)  */
        18176,    /*  a(5)  */
        114056,   /*  a(6)  */
        242176,   /*  a(7)  */
        1784720,  /*  a(8)  */
        3777536,  /*  a(9)  */
        30197792  /*  a(10) */
    };

    /* 2. memoisation storage */
    static seq_t *cache = NULL;   /* cache[0] unused; cache[1] .. cache[cache_size] valid */
    static idx_t cache_size = 0;  /* last index stored */

    /* 3. return immediately if we already know the value */
    if (n <= cache_size && cache != NULL) return cache[n];

    /* 4. initialisation of the cache (up to index 10) */
    if (cache == NULL || cache_size < 10) {
        size_t new_cap = 11;          /* indices 0..10 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                   /* out of memory – caller should handle */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = 1; i <= 10; ++i) cache[i] = init[i];
        cache_size = 10;
    }

    /* 5. extend cache up to the requested index n */
    if (n > cache_size) {
        /* allocate space for indices 0 .. n */
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        /* compute new values using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* terms in the recurrence */
            seq_t t1 = ((seq_t)32)  * cache[i - 2];
            seq_t t2 = ((seq_t)308) * cache[i - 4];
            seq_t t3 = ((seq_t)1072) * cache[i - 6];
            seq_t t4 = ((seq_t)1152) * cache[i - 8];

            /* final value: (t1 + t3) - (t2 + t4)  ensures no underflow */
            cache[i] = (t1 + t3) - (t2 + t4);
        }
        cache_size = n;
    }

    return cache[n];
}
