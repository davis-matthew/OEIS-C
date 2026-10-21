/*
 * A213393  Number of (w,x,y) with all terms in {0,...,n} and 2*max(w,x,y) >
 *          3*min(w,x,y).
 * https://oeis.org/A213393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=6, a(2)=24, a(3)=54, a(4)=108, a(5)=192, a(6)=300,
 *            a(7)=450 for n ≥ 8:
 *            a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values */
static idx_t cache_len = 0;      /* number of terms stored (cache[0]..cache[cache_len-1]) */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;

    /* Compute all newly required terms */
    for (size_t i = cache_len; i < new_len; ++i)
    {
        /* Base values */
        if (i == 0)          cache[i] = 0;
        else if (i == 1)     cache[i] = 6;
        else if (i == 2)     cache[i] = 24;
        else if (i == 3)     cache[i] = 54;
        else if (i == 4)     cache[i] = 108;
        else if (i == 5)     cache[i] = 192;
        else if (i == 6)     cache[i] = 300;
        else if (i == 7)     cache[i] = 450;
        else                 /* recurrence for i ≥ 8 */
            cache[i] = 2*cache[i-1] - cache[i-2] + 2*cache[i-3]
                       - 4*cache[i-4] + 2*cache[i-5] - cache[i-6]
                       + 2*cache[i-7] - cache[i-8];
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
