/*
 * A186616  Number of (n+5)X11 binary arrays with every 6X6 subblock commuting
 *          with each horizontal and vertical neighbor 6X6 subblock.
 * https://oeis.org/A186616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-6) - a(n-7) - a(n-10) +
 *            a(n-11) + a(n-12) - a(n-16) (n > 23) Initial values (for 1 ≤ n ≤
 *            23): a(1)=15228305, a(2)=451621, a(3)=614995, a(4)=1478651,
 *            a(5)=3608091, a(6)=5490491, a(7)=6009659, a(8)=6088635,
 *            a(9)=6098495, a(10)=6108363, a(11)=6118259, a(12)=6128...
 * Range:     u8 none, u16 none, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[23] = {
    15228305ULL, 451621ULL, 614995ULL, 1478651ULL,
    3608091ULL, 5490491ULL, 6009659ULL, 6088635ULL,
    6098495ULL, 6108363ULL, 6118259ULL, 6128419ULL,
    6141303ULL, 6166351ULL, 6262063ULL, 6714603ULL,
    8035411ULL, 11072195ULL, 15797367ULL, 21906448ULL,
    28096053ULL, 34652354ULL, 42086819ULL
};

static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1), … */
static size_t cache_size = 0; /* number of valid indices in cache (>= n+1) */

seq_t a(idx_t n)
{
    size_t sn = (size_t)n;

    /* Initialize cache with base values if first call */
    if (cache_size == 0) {
        cache_size = 24;               /* indices 0 … 23 */
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        cache[0] = 0;                  /* placeholder for 0‑th index */
        for (size_t i = 1; i <= 23; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* Return if already computed */
    if (sn < cache_size)
        return cache[sn];

    /* Grow cache to accommodate up to n */
    size_t old_size = cache_size;
    cache_size = sn + 1;
    cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));

    /* Compute missing terms using the recurrence */
    for (size_t i = old_size; i <= sn; ++i) {
        cache[i] =
            cache[i - 1] + cache[i - 4] - cache[i - 5] + cache[i - 6]
          - cache[i - 7] - cache[i -10] + cache[i -11] + cache[i -12]
          - cache[i -16];
    }

    return cache[sn];
}
