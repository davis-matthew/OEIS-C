/*
 * A186470  Number of (n+1)X8 0..2 arrays with every 2X2 subblock commuting
 *          with each of its horizontal and vertical 2X2 subblock neighbors.
 * https://oeis.org/A186470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + 4*a(n-4) - 2*a(n-5) for
 *            n>16 initial values: a(1)=3078, a(2)=2937, a(3)=9982,
 *            a(4)=14883, a(5)=40626, a(6)=49407, a(7)=136086, a(8)=170811,
 *            a(9)=431658, a(10)=564459, a(11)=1356918, a(12)=1931673,
 *            a(13)=4382606, a(14)=6664633, a(15)=14283484, a(16)=22714258
 * Range:     u8 none, u16 n <= 6, u32 n <= 24, u64 n <= 61, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 1..16. */
    static const seq_t init[16] = {
        3078u, 2937u, 9982u, 14883u, 40626u,
        49407u, 136086u, 170811u, 431658u, 564459u,
        1356918u, 1931673u, 4382606u, 6664633u, 14283484u, 22714258u
    };

    /* Memoization state. */
    static seq_t *cache = NULL;      /* stored terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity of cache */

    /* Ensure the cache is initialized. */
    if (cache == NULL) {
        cache_cap = 16;
        cache_len = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();   /* allocation failure */
        for (size_t i = 0; i < 16; ++i) cache[i] = init[i];
    }

    /* Expand the allocation if necessary. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 16;
        while (new_cap < (size_t)n + 1U) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i <= 16) {          /* should already be initialized */
            cache[i - 1] = init[i - 1];
        } else {                /* use the recurrence */
            seq_t val = 2 * cache[i - 2] + cache[i - 3];
            val -= 4 * cache[i - 4];
            val += 4 * cache[i - 5];
            val -= 2 * cache[i - 6];
            cache[i - 1] = val;
        }
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
