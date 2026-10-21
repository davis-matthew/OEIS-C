/*
 * A233646  Number of (n+1)X(1+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 14.
 * https://oeis.org/A233646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 2*a(n-1) + 31*a(n-2) - 36*a(n-3) - 261*a(n-4) 
   + 206*a(n-5) + 714*a(n-6) - 356*a(n-7) - 528*a(n-8) + 256*a(n-9) */
/* initial terms for n = 1 .. 9 are taken from the OEIS entry */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    (seq_t)0,        /* unused index 0 */
    (seq_t)144,      /* a(1) */
    (seq_t)772,      /* a(2) */
    (seq_t)3984,     /* a(3) */
    (seq_t)20936,    /* a(4) */
    (seq_t)108864,   /* a(5) */
    (seq_t)570208,   /* a(6) */
    (seq_t)2971200,  /* a(7) */
    (seq_t)15541312, /* a(8) */
    (seq_t)81063040  /* a(9) */
};

static seq_t *cache = NULL;   /* cache[0] unused, cache[1..] holds computed terms */
static idx_t  cache_size = 0; /* highest index computed so far */
static size_t cache_cap   = 0; /* allocated capacity (>0 when cache allocated) */

static void ensure_capacity(idx_t needed)
{
    if (cache_cap > needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1, with O(n) incremental computation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* unused index 0 */

    ensure_capacity(n);

    if (n <= cache_size)
        return cache[n];

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_vals[i];
        } else {
            seq_t r = (seq_t)0;
            r += (seq_t)2   * cache[i-1];
            r += (seq_t)31  * cache[i-2];
            r -= (seq_t)36  * cache[i-3];
            r -= (seq_t)261 * cache[i-4];
            r += (seq_t)206 * cache[i-5];
            r += (seq_t)714 * cache[i-6];
            r -= (seq_t)356 * cache[i-7];
            r -= (seq_t)528 * cache[i-8];
            r += (seq_t)256 * cache[i-9];
            cache[i] = r;
        }
    }
    cache_size = n;
    return cache[n];
}
