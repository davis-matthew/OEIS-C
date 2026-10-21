/*
 * A224370  Number of n X 4 0..2 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 58*a(n-2) + 105*a(n-3) + 20*a(n-4) -
 *            183*a(n-5) + 220*a(n-6) + 240*a(n-7) with a(1)=46, a(2)=698,
 *            a(3)=7608, a(4)=77793, a(5)=800309, a(6)=8297747, a(7)=86251004,
 *            a(8)=896856330
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static const seq_t init[9] = {
    0,                                   /* 0 unused */
    46,                                  /* a(1) */
    698,                                 /* a(2) */
    7608,                                /* a(3) */
    77793,                               /* a(4) */
    800309,                              /* a(5) */
    8297747,                             /* a(6) */
    86251004,                            /* a(7) */
    896856330                            /* a(8) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not part of the sequence */

    /* Allocate or extend cache as needed */
    if (cache == NULL) {
        cache_cap = (n > 8 ? n : 8);
        cache = (seq_t *)calloc((size_t)(cache_cap + 1), sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= cache_cap && i <= 8; ++i)
            cache[i] = init[i];
        cache_len = cache_cap;
    } else if (n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* initialize new entries up to 8 if they are not yet set */
        for (idx_t i = cache_len + 1; i <= cache_cap && i <= 8; ++i)
            cache[i] = init[i];
        cache_cap = n;
        if (cache_len < 8 && cache_cap >= 8) cache_len = 8;
    }

    if (n <= cache_len) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            seq_t t = cache[i - 1] * (seq_t)15;
            t += cache[i - 3] * (seq_t)105;
            t += cache[i - 4] * (seq_t)20;
            t += cache[i - 6] * (seq_t)220;
            t += cache[i - 7] * (seq_t)240;
            t -= cache[i - 2] * (seq_t)58;
            t -= cache[i - 5] * (seq_t)183;
            cache[i] = t;
        }
    }
    cache_len = n;
    return cache[n];
}
