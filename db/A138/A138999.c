/*
 * A138999  First differences of Frobenius numbers for 8 successive numbers
 *          A138988.
 * https://oeis.org/A138999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138999 – first differences of Frobenius numbers for 8 successive numbers
 * Recurrence:   a(n) = 2*a(n-7) - a(n-14)   for n > 14
 * Initial values (n = 1 … 14):
 *   1, 1, 1, 1, 1, 1, 10, 2, 2, 2, 2, 2, 2, 18
 * This function memoises the sequence and computes any a(n) (n ≥ 1) in O(n).
 */

#include <stdlib.h>

static seq_t init_vals[14] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)1, (seq_t)1, (seq_t)10, (seq_t)2,
    (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)18
};

/* Memoisation storage */
static seq_t *cache   = NULL;   /* array of a(1), a(2), ... */
static idx_t  last   = 0;      /* number of terms currently cached (0 if none) */
static size_t cap    = 0;      /* allocated capacity */

static void grow_cache(idx_t needed)
{
    /* Allocate or grow the cache to hold at least 'needed' terms. */
    if (cache == NULL) {
        cap = needed > 16 ? needed : 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort(); /* memory error */
        for (idx_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
        last = 14;
    }
    while (cap < (size_t)needed) {
        size_t new_cap = cap * 2;
        if (new_cap < cap) new_cap = needed;          /* prevent overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but guard against underflow */

    /* Ensure cache covers the requested index */
    if (last < n) {
        grow_cache(n);
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i > 14) {
                /* Using 0‑based cache indices: element a(k) is at cache[k-1]   */
                cache[i - 1] = 2 * cache[i - 8] - cache[i - 15];   /* a(i-7) and a(i-14)  */
            }
        }
        last = n;
    }

    return cache[n - 1];
}
