/*
 * A135258  Inverse binomial transform of A131666 after removing A131666(0) =
 *          0.
 * https://oeis.org/A135258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) + a(n-3) + 2*a(n-4); Initial terms: a(0)=0,
 *            a(1)=1, a(2)=-1, a(3)=2
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 33, i64 n <= 65, i128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cap   = 0;     /* allocated size of cache */
static idx_t sz    = 0;     /* number of terms actually stored */

/* Ensure the cache has space for at least needed elements */
static void ensure_cache(idx_t needed)
{
    if (cap >= needed) return;
    idx_t newcap = cap ? cap << 1 : 4;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
    if (!tmp) abort();                 /* out‑of‑memory */
    cache = tmp;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    /* initialise first four terms once */
    if (sz == 0) {
        ensure_cache(4);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache[3] = (seq_t)2;
        sz = 4;
    }

    if (n < sz) return cache[n];

    /* extend cache up to n */
    ensure_cache(n + 1);
    for (idx_t i = sz; i <= n; ++i) {
        seq_t term = -2 * cache[i - 1] + cache[i - 3] + 2 * cache[i - 4];
        cache[i] = term;
    }
    sz = n + 1;
    return cache[n];
}
