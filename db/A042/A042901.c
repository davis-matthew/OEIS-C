/*
 * A042901  Denominators of continued fraction convergents to sqrt(982).
 * https://oeis.org/A042901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A042901: Denominators of continued‑fraction convergents to sqrt(982).
    Recurrence: a(n) = 17674*a(n-6) - a(n-12)   for n > 11
    Initial values (n = 0 … 11):
        1, 2, 3, 92, 95, 282, 17579, 35440, 53019, 1626010, 1679029, 4984068
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of values actually stored */

    /* --- ensure storage is large enough for required indices --- */
    size_t need      = (size_t)n + 1;          /* we need indices 0 … n */
    size_t min_cap   = 12;                     /* we must hold the 12 initial terms */
    if (need < min_cap) need = min_cap;        /* never allocate < 12 */

    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache      = tmp;
        cache_cap  = need;
    }

    /* --- initialise the first 12 terms on first call or after reallocation --- */
    if (cache_len == 0) {
        static const seq_t init[12] = {
            (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)92,
            (seq_t)95, (seq_t)282, (seq_t)17579, (seq_t)35440,
            (seq_t)53019, (seq_t)1626010, (seq_t)1679029, (seq_t)4984068
        };
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        cache_len = 12;
    }

    /* --- return cached value if already computed --- */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* --- compute new terms up to n using the recurrence --- */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* i-6 and i-12 are valid because cache_len >= 12 at this point. */
        cache[i] = cache[i-6] * (seq_t)17674 - cache[i-12];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
