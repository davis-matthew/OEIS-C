/*
 * A184755  Half the number of n X 3 binary arrays with no 1 having an
 *          adjacent 1 both above and to its left.
 * https://oeis.org/A184755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical Recurrence:
   a(n) = 5*a(n-1) + 9*a(n-2) + 4*a(n-3)   for n ≥ 4
   a(1) = 4,  a(2) = 25,  a(3) = 163 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache      = NULL; /* 1‑based array, cache[0] unused   */
static idx_t cache_len   = 0;    /* highest index that has been computed */
static size_t cache_cap = 0;    /* allocated capacity (elements, including index 0) */

static void grow_cache(idx_t needed)
{
    /* Ensure capacity for indices up to 'needed' (inclusive) */
    size_t required = (size_t)needed + 1;                 /* +1 because index 0 is unused */
    if (required <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < required)
        new_cap <<= 1;

    seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_buf)
        return; /* allocation failure; caller will handle 0 result */

    if (cache) {
        memcpy(new_buf, cache, (cache_len + 1) * sizeof(seq_t));
        free((void *)cache);
    }
    cache = new_buf;
    cache_cap = new_cap;
}

static void init_base_cases(void)
{
    if (cache_len >= 3)
        return;
    grow_cache(3); /* ensure space for indices 1..3 */
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)25;
    cache[3] = (seq_t)163;
    cache_len = 3;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for indices below the offset */

    init_base_cases();
    if (n <= cache_len)
        return cache[n];

    grow_cache(n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = ((seq_t)5) * cache[i - 1]
                  + ((seq_t)9) * cache[i - 2]
                  + ((seq_t)4) * cache[i - 3];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
