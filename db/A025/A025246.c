/*
 * A025246  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 1, 0, 1, 1.
 * https://oeis.org/A025246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)*a(n-1) + a(2)*a(n-2) + … + a(n-3)*a(3) for n ≥ 4
 *            Initial terms: a(1) = 1, a(2) = 0, a(3) = 1, a(4) = 1
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 34, u64 n <= 62, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array, cache[0] unused */
static idx_t cache_len = 0;   /* max n that has a computed value */

/* Return a(n).  Values are memoised so that subsequent calls are cheap. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined, but safe */

    /* ensure cache large enough to hold up to n */
    if (!cache || cache_len < n) {
        size_t new_size = (size_t)n + 1;     /* one past the highest index */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */

        cache = tmp;
        /* initialise new entries when the cache grows */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 4) {                    /* fixed initial values */
                if (i == 1) cache[i] = (seq_t)1;
                else if (i == 2) cache[i] = (seq_t)0;
                else if (i == 3) cache[i] = (seq_t)1;
                else            cache[i] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;                   /* compute a(i) */
            for (idx_t k = 1; k <= i - 3; ++k) {
                sum += cache[k] * cache[i - k];
            }
            cache[i] = sum;
        }
        cache_len = n;
    }
    return cache[n];
}
