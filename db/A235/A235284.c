/*
 * A235284  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 15*a(n-3) + 4*a(n-4) + 18*a(n-5) -
 *            12*a(n-6) (with a(1)=68, a(2)=104, a(3)=148, a(4)=248, a(5)=380,
 *            a(6)=680)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization table */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t cache_max = 0;          /* largest n computed so far */

static inline void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 8;          /* some slack */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset not used */

    ensure_cache(n);

    if (n <= cache_max) return cache[n];

    /* initialize base cases if needed */
    if (cache_max < 6) {
        cache[1] = (seq_t)68;
        cache[2] = (seq_t)104;
        cache[3] = (seq_t)148;
        cache[4] = (seq_t)248;
        cache[5] = (seq_t)380;
        cache[6] = (seq_t)680;
        if (cache_max < 6) cache_max = 6;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 6) continue;           /* already set */

        /* recurrence: a[i] = 3*a[i-1] + 3*a[i-2] - 15*a[i-3] + 4*a[i-4] + 18*a[i-5] - 12*a[i-6] */
        seq_t val = 0;
        val += cache[i-1] * (seq_t)3;
        val += cache[i-2] * (seq_t)3;
        val -= cache[i-3] * (seq_t)15;
        val += cache[i-4] * (seq_t)4;
        val += cache[i-5] * (seq_t)18;
        val -= cache[i-6] * (seq_t)12;
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
