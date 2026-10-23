/*
 * A116820  Number of permutations of length n which avoid the patterns 2341,
 *          3241, 4213.
 * https://oeis.org/A116820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 23*a(n-2) + 29*a(n-3) - 14*a(n-4) + a(n-5);
 *            initial terms: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* cache[0] holds a(1) */
static size_t cache_cap = 0;      /* current allocated capacity (in entries) */
static idx_t  last_computed = 0;  /* number of entries already computed */

static void
ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!tmp) exit(1); /* memory allocation failure */
        cache = tmp;
        cache_cap = (size_t)n;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;  /* sequence defined for n >= 1 */

    if (n <= last_computed)
        return cache[(size_t)n - 1];

    ensure_cache(n);

    for (idx_t i = last_computed; i < n; ++i) {
        seq_t val;
        if (i == 0)
            val = 1;
        else if (i == 1)
            val = 2;
        else if (i == 2)
            val = 6;
        else if (i == 3)
            val = 21;
        else if (i == 4)
            val = 75;
        else {
            /* recurrence for a(i+1) where i>=5 (0‑based index) */
            val = (seq_t)8   * cache[(size_t)i - 1]
                 - (seq_t)23  * cache[(size_t)i - 2]
                 + (seq_t)29  * cache[(size_t)i - 3]
                 - (seq_t)14  * cache[(size_t)i - 4]
                 +           cache[(size_t)i - 5];
        }
        cache[(size_t)i] = val;
    }

    last_computed = n;
    return cache[(size_t)n - 1];
}
