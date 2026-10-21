/*
 * A234210  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the squares of all six edge and diagonal
 *          differences equal to 11.
 * https://oeis.org/A234210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-2) - 112*a(n-4) * with a(1)=72, a(2)=308,
 *            a(3)=1248, a(4)=5344
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* memoisation table */
static size_t cache_len = 0;     /* current table size (indices 0..cache_len-1) */

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);            /* indices 0 … n are required */
    if (cache == NULL) {                       /* first allocation */
        cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!cache) abort();
        cache_len = needed;
        return;
    }
    if (needed <= cache_len) return;           /* already enough space */

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();
    /* zero the newly allocated part so that “0” means “uncomputed” */
    for (size_t i = cache_len; i < needed; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* a(0) is not defined in OEIS */
    ensure_cache(n);

    if (cache[n] != 0) return cache[n];       /* already computed */

    /* fill cache up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;          /* already known */

        if (i == 1)      cache[i] = (seq_t)72;
        else if (i == 2) cache[i] = (seq_t)308;
        else if (i == 3) cache[i] = (seq_t)1248;
        else if (i == 4) cache[i] = (seq_t)5344;
        else
            cache[i] = (seq_t)24 * cache[i - 2] - (seq_t)112 * cache[i - 4];
    }
    return cache[n];
}
