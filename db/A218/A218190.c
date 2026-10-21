/*
 * A218190  Hilltop maps: number of nX2 binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal or
 *          vertical neighbor in a random 0..3 nX2 array.
 * https://oeis.org/A218190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)+3*a(n-3)+3*a(n-4)+3*a(n-5) *
 *            +2*a(n-6)+2*a(n-7)-a(n-8)-a(n-9)-a(n-10)-a(n-11)-a(n-12)-a(n-13),
 *            * with a(1)=3, a(2)=15, a(3)=63, a(4)=251, a(5)=1001, a(6)=4001,
 *            * a(7)=16001, a(8)=63985, a(9)=255867, a(10)=1023171,
 *            a(11)=4091495, * a(12)=16361227, a(13)=65425905. Off‑by‑on...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Initial 13 values of the sequence */
static const seq_t base_vals[13] = {
    (seq_t)3, (seq_t)15, (seq_t)63, (seq_t)251, (seq_t)1001,
    (seq_t)4001, (seq_t)16001, (seq_t)63985, (seq_t)255867,
    (seq_t)1023171, (seq_t)4091495, (seq_t)16361227, (seq_t)65425905
};

/* Cache for computed terms; index 0 is unused. */
static seq_t *cache   = NULL;
static idx_t cache_sz = 0;        /* size of allocated array (indices 0..size-1) */
static idx_t computed = 0;        /* highest index that has a valid value */

/* Allocate (or grow) cache to hold indices up to n inclusive,
 * and initialise the base values if they are not yet present. */
static void ensure_cache_size(idx_t n)
{
    /* need space for indices 0..n */
    idx_t need = n + 1;
    if (need > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
        cache = tmp;
        cache_sz = need;
    }

    /* initialise base values 1..13 if not already done */
    if (computed < 13) {
        /* we may start from 1 up to 13 or up to cache_sz-1, whichever is smaller */
        idx_t last = (cache_sz - 1 < 13) ? (cache_sz - 1) : 13;
        for (idx_t i = 1; i <= last; ++i)
            cache[i] = base_vals[i - 1];
        computed = last;
    }
}

/* Public function implementing the recurrence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* behaviour for n<1 is irrelevant */

    if (n <= computed)
        return cache[n];

    ensure_cache_size(n);

    /* Start computing from the next index after the highest computed,
       but we cannot compute before n=14. */
    idx_t start = (computed >= 14) ? (computed + 1) : 14;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i - 1]
                  + (seq_t)3 * cache[i - 2]
                  + (seq_t)3 * cache[i - 3]
                  + (seq_t)3 * cache[i - 4]
                  + (seq_t)3 * cache[i - 5]
                  + (seq_t)2 * cache[i - 6]
                  + (seq_t)2 * cache[i - 7]
                  -            cache[i - 8]
                  -            cache[i - 9]
                  -            cache[i -10]
                  -            cache[i -11]
                  -            cache[i -12]
                  -            cache[i -13];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
