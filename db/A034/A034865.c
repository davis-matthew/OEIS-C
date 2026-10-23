/*
 * A034865  a(n) = n!*(n-4)/2.
 * https://oeis.org/A034865
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n-4)/2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* ensure cache holds values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < 4 || n < cache_len)
        return;

    size_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache)
        return;          /* allocation failed – uncommon in test harness */

    /* fill new entries */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i < 4)
            continue;

        if (i == 4) {
            cache[4] = 0;
        } else if (i == 5) {
            seq_t val = (seq_t)120;          /* 5! */
            val /= (seq_t)2;                 /* (n-4)/2 part */
            cache[5] = val;
        } else { /* i >= 6 */
            seq_t val = cache[i - 1];
            val *= (seq_t)i;
            val *= (seq_t)(i - 4);
            val /= (seq_t)(i - 5);           /* division exact for i >= 6 */
            cache[i] = val;
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* behavior below offset does not matter */

    ensure_cache(n);
    return cache[n];
}
