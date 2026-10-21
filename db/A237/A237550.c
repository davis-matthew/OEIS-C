/*
 * A237550  Number of (n+1) X (1+1) 0..2 arrays with the maximum minus the
 *          upper median of every 2 X 2 subblock differing from its horizontal
 *          and vertical neighbors by exactly one.
 * https://oeis.org/A237550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 192*a(n-4) + 608*a(n-6) + 400*a(n-8) for n > 9 with
 *            initial values: a(1)=81, a(2)=312, a(3)=1240, a(4)=4752,
 *            a(5)=19128, a(6)=72736, a(7)=290112, a(8)=1110080, a(9)=4459296
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* store a[0..max] values, index 0 unused */
static idx_t cache_cap = 0;   /* allocated capacity (highest index + 1) */

static const seq_t init_vals[10] = {0, 81, 312, 1240, 4752,
                                    19128, 72736, 290112,
                                    1110080, 4459296};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based; ignore n=0 */

    idx_t need = n + 1;           /* we need indices 1..n */

    /* Allocate or grow the cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)calloc(need, sizeof(seq_t));
        if (!cache) return 0;     /* allocation failure – return 0 */
        cache_cap = need;
    } else if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure – return 0 */
        cache = tmp;
        for (idx_t i = cache_cap; i < need; ++i) cache[i] = 0;
        cache_cap = need;
    }

    /* Initialise base values (indices 1..9) */
    for (idx_t i = 1; i <= 9 && i < cache_cap; ++i) {
        if (!cache[i]) cache[i] = init_vals[i];
    }

    /* Compute values using the recurrence */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 9) continue;          /* base values already set */
        if (!cache[i]) {
            cache[i] = 192 * cache[i-4]
                      + 608 * cache[i-6]
                      + 400 * cache[i-8];
        }
    }

    return cache[n];
}
