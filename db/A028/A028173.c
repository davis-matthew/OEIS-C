/*
 * A028173  Expansion of 1/((1-5*x)*(1-6*x)*(1-8*x)*(1-12*x)).
 * https://oeis.org/A028173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028173: Expansion of 1/((1-5*x)*(1-6*x)*(1-8*x)*(1-12*x))
   Recurrence: a(0)=1, a(1)=31, a(2)=615, a(3)=9995
   a(n)=31*a(n-1)-346*a(n-2)+1656*a(n-3)-2880*a(n-4)   (n>=4)
   The recurrence follows from the denominator polynomial after
   clearing the generating function. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values   */
static idx_t cache_cap = 0;      /* allocated capacity       */
static idx_t cache_len = 0;      /* next index to compute    */

/* Ensure the cache holds indices [0..n] initialized via the recurrence */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already computed */

    /* Grow storage if needed */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;           /* at least four slots */
        new_cap = (new_cap > cache_cap * 2) ? new_cap : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first few entries if the cache was empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;                 /* a(0) */
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 1) {
        cache[1] = (seq_t)31;                /* a(1) */
        cache_len = 2;
    }
    if (cache_len == 2 && n >= 2) {
        cache[2] = (seq_t)615;               /* a(2) */
        cache_len = 3;
    }
    if (cache_len == 3 && n >= 3) {
        cache[3] = (seq_t)9995;              /* a(3) */
        cache_len = 4;
    }

    /* Use the recurrence to fill remaining entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = (seq_t)31   * cache[i-1] + (seq_t)1656 * cache[i-3];
        seq_t neg = (seq_t)346  * cache[i-2] + (seq_t)2880 * cache[i-4];
        cache[i] = pos - neg;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
