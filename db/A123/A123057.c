/*
 * A123057  Values x of solutions (x, y) to the Diophantine equation (x-y)^4 -
 *          8*x*y = 0 with x >= y.
 * https://oeis.org/A123057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*a(n-1) - 206*a(n-2) + 40*a(n-3) - a(n-4) (n >= 4)
 *            Initial values: a(0)=0, a(1)=8, a(2)=216, a(3)=7000. The
 *            function maintains a cache so that repeated calls are efficient
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of cached terms */
static size_t cache_len = 0;     /* number of terms currently stored (0‑based) */
static size_t cache_cap = 0;     /* capacity of the array */

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    /* Grow capacity: double or at least enough for needed. */
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < needed) new_cap = needed;

    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);              /* allocation failed */

    if (cache) {
        /* Copy existing terms */
        for (size_t i = 0; i < cache_len; ++i)
            new_buf[i] = cache[i];
        free(cache);
    }

    cache      = new_buf;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* Constrain index for array operations */
    size_t idx = (size_t)n;
    size_t need = idx + 1;              /* number of elements required */

    /* Expand cache if necessary */
    if (need > cache_len) {
        ensure_cache(need);

        /* Compute missing terms up to index `idx`. */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t term;

            if (i == 0) {
                term = (seq_t)0;
            } else if (i == 1) {
                term = (seq_t)8;
            } else if (i == 2) {
                term = (seq_t)216;
            } else if (i == 3) {
                term = (seq_t)7000;
            } else {
                /* a(i) = 40*a(i-1) - 206*a(i-2) + 40*a(i-3) - a(i-4) */
                seq_t tmp = cache[(size_t)(i - 1)];
                tmp *= (seq_t)40;

                seq_t tmp3 = cache[(size_t)(i - 3)];
                tmp3 *= (seq_t)40;
                tmp += tmp3;

                seq_t tmp2 = cache[(size_t)(i - 2)];
                tmp2 *= (seq_t)206;
                tmp -= tmp2;

                tmp -= cache[(size_t)(i - 4)];

                term = tmp;
            }

            cache[(size_t)i] = term;
        }

        cache_len = need;
    }

    return cache[idx];
}
