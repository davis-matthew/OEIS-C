/*
 * A213400  Number of (w,x,y) with all terms in {0,...,n} and w < R < 2*w,
 *          where R = range{w,x,y} = max(w,x,y)-min(w,x,y).
 * https://oeis.org/A213400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=2*a(n-2)+2*a(n-3)-a(n-4)-4*a(n-5)-a(n-6)+2*a(n-7)+2*a(n-8)-a(n-10)
   initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=2,a(4)=6,a(5)=19,a(6)=29,a(7)=52,a(8)=81,a(9)=116 */

#include <stdlib.h>       /* malloc, free, realloc */

static const seq_t initial[10] = {0,0,0,2,6,19,29,52,81,116};

/* Cache for computed terms, 0..last_computed */
static seq_t *cache      = NULL;   /* pointer to cache array */
static size_t cache_size = 0;      /* allocated size */
static size_t last_computed = 0;   /* highest index computed so far (>=9 after init) */

/* Ensure the cache exists and is large enough for index n */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        /* First allocation with space for 0..9 */
        cache_size = 10;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return;          /* allocation failure – will return 0 */
        for (size_t i = 0; i < 10; ++i) cache[i] = initial[i];
        last_computed = 9;
    }

    size_t required = (size_t)n + 1;
    if (cache_size < required) {
        seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return;              /* keep old cache on failure */
        cache = tmp;
        cache_size = required;
    }
}

/* Return a(n) – memoised, works for any call order */
seq_t a(idx_t n)
{
    if (n < 10) return initial[(size_t)n];

    ensure_cache(n);
    if (!cache) return 0;              /* allocation failure */

    if ((size_t)n <= last_computed) return cache[(size_t)n];

    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = 2 * cache[i-2] + 2 * cache[i-3];
        pos += 2 * cache[i-7] + 2 * cache[i-8];
        seq_t neg = cache[i-4] + 4 * cache[i-5];
        neg += cache[i-6] + cache[i-10];
        cache[i] = pos - neg;
    }
    last_computed = n;
    return cache[(size_t)n];
}
