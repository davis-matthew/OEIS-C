/*
 * A238383  Row sums of triangle in A139040.
 * https://oeis.org/A238383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-6) - a(n-7), a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=8, a(6)=11
 * Range:     u8 n <= 21, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Memoisation cache for the sequence */
static seq_t *cache   = NULL;
static idx_t   cap    = 0;          /* current capacity of cache */
static idx_t   last   = (idx_t)-1;  /* index of the last cached value */

/* Ensure the cache can store values up to index 'need' (inclusive) */
static void
ensure_cap(idx_t need)
{
    if (need < cap)
        return;                     /* already sufficient */

    idx_t new_cap = (need + 1) * 2;  /* over‑allocate to keep O(1) amortised */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)                  /* allocation failed – abort */
        abort();
    cache = new_cache;
    /* initialise new slots to zero */
    for (idx_t i = cap; i < new_cap; ++i)
        cache[i] = 0;
    cap = new_cap;
}

/* Initialise the cache with the first seven terms of the sequence */
static void
init_cache(void)
{
    if (cache == NULL) {
        ensure_cap(6);               /* need indices 0 … 6 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)11;
        last = 6;
    }
}

/* Public function: computes a(n) */
seq_t
a(idx_t n)
{
    init_cache();                   /* guarantee base terms are ready */

    if (n <= last)
        return cache[n];

    /* Need to compute values up to n */
    ensure_cap(n);

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;
        /* add a(i-1), a(i-2), a(i-6) */
        if (i >= 1) val += cache[i - 1];
        if (i >= 2) val += cache[i - 2];
        if (i >= 6) val += cache[i - 6];
        /* subtract a(i-3), a(i-7) */
        if (i >= 3) val -= cache[i - 3];
        if (i >= 7) val -= cache[i - 7];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
