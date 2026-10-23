/*
 * A025974  Expansion of 1/((1-2*x)*(1-4*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A025974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 193*a(n-2) + 606*a(n-3) - 616*a(n-4) with
 *            a(0)=1, a(1)=24, a(2)=383, a(3)=5166
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for memorised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* number of slots currently allocated */
    static size_t cache_len = 0;      /* number of values stored (highest used index + 1) */

    /* Ensure space for the first four values */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        cache[2] = (seq_t)383;
        cache[3] = (seq_t)5166;
        cache_len = 4;
    }

    /* Expand cache if needed */
    if (n >= cache_len) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) {
            new_cap = (new_cap > 0 ? new_cap * 2 : 4);
        }
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t val = cache[i - 1] * (seq_t)24;
            val -= cache[i - 2] * (seq_t)193;
            val += cache[i - 3] * (seq_t)606;
            val -= cache[i - 4] * (seq_t)616;
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
