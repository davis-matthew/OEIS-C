/*
 * A231317  Number of (n+1) X (1+1) 0..2 arrays with no element equal to a
 *          strict majority of its diagonal and antidiagonal neighbors, with
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A231317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 12*a(n-2) - 8*a(n-3) Base values: a(1)=6,
 *            a(2)=24, a(3)=216
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache of computed terms */
static idx_t cache_size = 0;  /* largest index cached so far */

static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure; abort – the build environment will treat this as a fatal error */
        abort();
    }
    cache = new_cache;
    /* Initialize new entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)6;
        } else if (i == 2) {
            cache[i] = (seq_t)24;
        } else if (i == 3) {
            cache[i] = (seq_t)216;
        } else {
            /* recurrence: a(i) = 6*a(i-1) + 12*a(i-2) - 8*a(i-3) */
            cache[i] = (seq_t)6 * cache[i-1] + (seq_t)12 * cache[i-2] - (seq_t)8 * cache[i-3];
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset starts at 1, but guard for 0 */
    ensure_cache(n);
    return cache[n];
}
