/*
 * A052974  Expansion of (1 - 2x)/(1 - 2x - x^2 - x^3 + 2x^4).
 * https://oeis.org/A052974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=1, a(3)=3;
   2*a(n) - a(n+1) - a(n+2) - 2*a(n+3) + a(n+4) = 0  (n >= 0)
   (so a(n+4) = a(n+1) + a(n+2) + 2*a(n+3) - 2*a(n)) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached terms */
static idx_t  cache_size = 0;    /* number of computed terms */
static idx_t  cache_cap  = 0;    /* allocated capacity */

/* Ensure the pool can hold at least 'needed' elements. */
static void
ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with the first five terms */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)1;   /* a(2) */
        cache[3] = (seq_t)3;   /* a(3) */
        cache[4] = (seq_t)5;   /* a(4) from the recurrence */
        cache_size = 5;
    }

    /* extend cache until it contains a(n) */
    while (cache_size <= n) {
        /* indices relative to the current stage
           a_n  = a(n)
           a_n1 = a(n+1)
           a_n2 = a(n+2)
           a_n3 = a(n+3) */
        seq_t a_n  = cache[cache_size - 4];
        seq_t a_n1 = cache[cache_size - 3];
        seq_t a_n2 = cache[cache_size - 2];
        seq_t a_n3 = cache[cache_size - 1];

        /* compute a(n+4) = a(n+1) + a(n+2) + 2*a(n+3) - 2*a(n) */
        seq_t temp = a_n1 + a_n2;
        temp += (seq_t)2 * a_n3;
        seq_t next = temp - (seq_t)2 * a_n;

        ensure_capacity(cache_size + 1);
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n];
}
