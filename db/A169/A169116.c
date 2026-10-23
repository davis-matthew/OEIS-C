/*
 * A169116  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1 * if n≤25 : a(n)=5·∑_{k=1}^{n}a(n-k)+2 * if n=26
 *            : a(26)=5·∑_{k=1}^{25}a(26-k)+1−15·a(0) * if n≥27 :
 *            a(n)=5·∑_{k=1}^{25}a(n-k)−15·a(n−26)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
static idx_t  cache_len = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;          /* minimal size */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* Initialise first term if this is the first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {          /* already set */
            cache[0] = (seq_t)1;
            continue;
        }

        /* sum of the previous up to 25 terms */
        seq_t sum = (seq_t)0;
        idx_t jmax = (i < 25) ? i : 25;
        for (idx_t j = 1; j <= jmax; ++j) {
            sum += cache[i - j];
        }

        /* apply the appropriate recurrence */
        if (i <= 25) {
            cache[i] = (seq_t)5 * sum + (seq_t)2;
        } else if (i == 26) {
            cache[i] = (seq_t)5 * sum + (seq_t)1 - (seq_t)15 * cache[0];
        } else {                      /* i >= 27 */
            cache[i] = (seq_t)5 * sum - (seq_t)15 * cache[i - 26];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
