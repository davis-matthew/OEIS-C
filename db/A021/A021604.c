/*
 * A021604  Expansion of 1/((1-x)*(1-3*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A021604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^(n+3) - 3*7^(n+4) + 3^(n+6) - 14)/1512 Recurrence
 *            for n>3: a(n) = 21*a(n-1) - 141*a(n-2) + 331*a(n-3) - 210*a(n-4)
 *            Initial values: a(0)=1, a(1)=21, a(2)=300, a(3)=3670
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow random‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* allocate or enlarge cache as needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise first four terms if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)300;
        cache[3] = (seq_t)3670;
        cache_len = 4;
    }

    /* compute missing terms up to n */
    if (n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 21*a(i-1) - 141*a(i-2) + 331*a(i-3) - 210*a(i-4) */
            seq_t val = 21 * cache[i - 1];
            val -= 141 * cache[i - 2];
            val += 331 * cache[i - 3];
            val -= 210 * cache[i - 4];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
