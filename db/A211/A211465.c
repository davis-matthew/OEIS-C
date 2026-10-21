/*
 * A211465  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having two, three or four distinct values for every i<=n and j<=n.
 * https://oeis.org/A211465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 45*a(n-2) + 40*a(n-3) + 46*a(n-4) + 8*a(n-5)
 *            (empirical recurrence). Initial values: a(1)=24, a(2)=116,
 *            a(3)=556, a(4)=2652, a(5)=12588. The function memoizes results
 *            and computes terms up to the largest requested index
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t first_vals[5] = {
    (seq_t)24,
    (seq_t)116,
    (seq_t)556,
    (seq_t)2652,
    (seq_t)12588
};

/* cached values: cache[i-1] == a(i) */
static seq_t *cache   = NULL;
static idx_t  cache_n = 0;   /* maximum index currently stored (>=1) */
static size_t cache_cap = 0; /* allocated size          */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n==0, return 0 as safe fallback */

    /* Ensure we have space for n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2; /* grow factor 2 */
        /* avoid overflow of size_t */
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        if (!cache)                       /* first allocation must fill first terms */
        {
            for (idx_t i = 1; i <= 5 && i <= (idx_t)(new_cap - 1); ++i)
                tmp[i-1] = first_vals[i-1];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill cache up to n.  Terms already computed will be left unchanged. */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        if (i <= 5) {
            cache[i-1] = first_vals[i-1];
            continue;
        }
        /* recurrence using only unsigned operations (sum of positives
           minus one product) */
        seq_t pos = cache[i-2] * 12;         /* a(i-1) */
        pos += cache[i-4] * 40;              /* a(i-3) */
        pos += cache[i-5] * 46;              /* a(i-4) */
        pos += cache[i-6] * 8;               /* a(i-5) */

        seq_t neg = cache[i-3] * 45;         /* a(i-2) */
        cache[i-1] = pos - neg;              /* unsigned subtraction; safe because result is positive */
    }
    cache_n = n;
    return cache[n-1];
}
