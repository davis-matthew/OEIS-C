/*
 * A166876  a(n) = a(n-1) + Fibonacci(n), a(1)=1983.
 * https://oeis.org/A166876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + Fibonacci(n), a(1) = 1983. Equivalent
 *            recurrence: a(n) = 2*a(n-1) – a(n-3), for n ≥ 4. The function
 *            memoises results in a static array, so calls may come in any
 *            order
 * Range:     u8 none, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* static memoisation array */
static size_t cache_cap  = 0;         /* current allocation size */
static size_t cache_len  = 0;         /* number of computed elements */

/* grow the cache to at least the needed capacity */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap << 1 : 8;
        while (newcap < needed)
            newcap <<= 1;
        seq_cache = (seq_t*)realloc(seq_cache,
                                    newcap * sizeof(*seq_cache));
        cache_cap = newcap;
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* n is 1-based; n>=1 is assumed */
    if (n == 0) return 0;            /* undefined, but guard anyway */

    size_t idx = (size_t)n;          /* number of elements needed */
    ensure_cache(idx);

    /* initialise the first three terms if not already present */
    if (cache_len < 3) {
        for (size_t i = cache_len; i < 3; ++i) {
            if (i == 0)          seq_cache[i] = (seq_t)1983;   /* a(1) */
            else if (i == 1)     seq_cache[i] = (seq_t)1984;   /* a(2) */
            else /* i==2 */     seq_cache[i] = (seq_t)1986;   /* a(3) */
        }
        cache_len = 3;
    }

    /* extend the cache up to the requested index */
    if (cache_len < idx) {
        for (size_t i = cache_len; i < idx; ++i) {
            /* recurrence: a(n) = 2*a(n-1) – a(n-3) */
            seq_cache[i] = (seq_t)2 * seq_cache[i-1] - seq_cache[i-3];
        }
        cache_len = idx;
    }

    return seq_cache[idx-1];   /* return a(n) */
}
