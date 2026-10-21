/*
 * A207702  Number of n X 7 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8), with a(1)=40, a(2)=1600,
 *            a(3)=13230, a(4)=61254, a(5)=206910, a(6)=571350, a(7)=1369900,
 *            a(8)=2956980
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t initial[9] = {
    0,          /* unused a(0) */
    40, 1600,
    13230,
    61254,
    206910,
    571350,
    1369900,
    2956980
};

static seq_t *cache = NULL;   /* 1‑based cache */
static size_t cache_size = 0; /* largest index stored */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* behavior below the offset is undefined */

    size_t idx = (size_t)n;
    if (idx <= cache_size && cache != NULL)
        return cache[idx];

    /* enlarge cache to hold up to index idx */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (idx + 1) * sizeof(seq_t));
    if (!new_cache)
        return 0;                     /* allocation failure – unlikely in tests */

    cache = new_cache;

    /* compute missing values */
    for (size_t i = cache_size + 1; i <= idx; ++i) {
        if (i <= 8) {
            cache[i] = initial[i];
        } else {
            seq_t val =
                cache[i-1] * 8
                - cache[i-2] * 28
                + cache[i-3] * 56
                - cache[i-4] * 70
                + cache[i-5] * 56
                - cache[i-6] * 28
                + cache[i-7] * 8
                - cache[i-8];
            cache[i] = val;
        }
    }

    cache_size = idx;
    return cache[idx];
}
