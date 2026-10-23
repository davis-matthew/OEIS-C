/*
 * A184489  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock sum
 *          greater than 4.
 * https://oeis.org/A184489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 11*a(n-2) - 4*a(n-3) - 12*a(n-4); initial
 *            values: a(1)=31, a(2)=165, a(3)=792, a(4)=3971
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t max_n = 0;

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate at least enough space for the first 5 indices
           (index 0 is unused) */
        cap = (n > 4) ? (size_t)(n + 1) : 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return;           /* memory error – unlikely */
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)165;
        cache[3] = (seq_t)792;
        cache[4] = (seq_t)3971;
        max_n   = 4;
    }

    if (n <= max_n) return;

    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return;               /* memory error – unlikely */
        memcpy(tmp, cache, (max_n + 1) * sizeof(seq_t));
        free(cache);
        cache = tmp;
        cap   = new_cap;
    }

    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)3 * cache[i - 1] +
            (seq_t)11 * cache[i - 2] -
            (seq_t)4 * cache[i - 3] -
            (seq_t)12 * cache[i - 4];
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
