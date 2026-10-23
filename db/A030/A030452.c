/*
 * A030452  Markov numbers satisfying region 5 (x=5) of the equation x^2 + y^2
 *          + z^2 = 3xyz.
 * https://oeis.org/A030452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-2) - a(n-4) Initial values: a(1) = 1, a(2) = 2,
 *            a(3) = 13, a(4) = 29
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] == a(1) */
static size_t cache_len = 0;  /* number of elements stored */

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    size_t new_len = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        abort();                /* memory allocation failure */
    }
    cache = tmp;

    for (idx_t i = (idx_t)cache_len; i < n; ++i) {
        if (i < 4) {                /* initial terms */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;  /* a(1) */
                case 1: cache[i] = (seq_t)2; break;  /* a(2) */
                case 2: cache[i] = (seq_t)13; break; /* a(3) */
                case 3: cache[i] = (seq_t)29; break; /* a(4) */
            }
        } else {                     /* recurrence for n ≥ 5 */
            cache[i] = (seq_t)(15 * cache[i-2] - cache[i-4]);
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n-1];
}
