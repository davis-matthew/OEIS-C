/*
 * A234984  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+6*a(n-2)-33*a(n-3)-24*a(n-4)+56*a(n-5)+34*a(n-6)-20*a(n-7)-4*a(n-8)
 *            Initial values: a(1)=90, a(2)=370, a(3)=1640, a(4)=7554,
 *            a(5)=35072, a(6)=163416, a(7)=762002, a(8)=3554334 This function
 *            caches computed terms so that repeated calls are O(1) after the
 *            cache has been extended to...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* 0‑based array storing a(1) at cache[0] */
static idx_t cache_len = 0;   /* number of terms currently cached */
static idx_t cache_cap = 0;   /* allocated capacity of cache */

/* initialise cache with first eight terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* allocate capacity for at least 8 terms */
    cache_cap = 8;
    cache_len  = 8;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    static const seq_t init[8] = {
        (seq_t)90,
        (seq_t)370,
        (seq_t)1640,
        (seq_t)7554,
        (seq_t)35072,
        (seq_t)163416,
        (seq_t)762002,
        (seq_t)3554334
    };
    memcpy(cache, init, sizeof(init));
}

/* enlarge cache to at least new_cap */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    idx_t alloc = new_cap < (cache_cap << 1) ? (cache_cap << 1) : new_cap;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * alloc);
    if (cache_len > 0)
        memcpy(new_cache, cache, sizeof(seq_t) * cache_len);
    free(cache);
    cache = new_cache;
    cache_cap = alloc;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) { /* offset: a(1) is first term; undefined for n==0 */
        return (seq_t)0;
    }

    init_cache();

    if (n > cache_len) {
        ensure_capacity(n);
        /* compute terms from cache_len up to n-1 (0‑based indices) */
        for (idx_t i = cache_len; i < n; ++i) {
            /* i corresponds to term number i+1 */
            idx_t idx = i; /* 0-based index */
            /* recurrence uses terms a(idx) ... a(idx-7)   which map to cache[idx] ... cache[idx-7]  */
            seq_t val = 0;
            val += 5 * cache[idx-1];
            val += 6 * cache[idx-2];
            val -= 33 * cache[idx-3];
            val -= 24 * cache[idx-4];
            val += 56 * cache[idx-5];
            val += 34 * cache[idx-6];
            val -= 20 * cache[idx-7];
            val -= 4  * cache[idx-8];
            cache[idx] = val;
        }
        cache_len = n;
    }

    return cache[n-1];
}
