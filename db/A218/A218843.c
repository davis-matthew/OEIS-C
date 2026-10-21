/*
 * A218843  Unmatched value maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..1 2 X n array.
 * https://oeis.org/A218843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) +
 *            2*a(n-5) + 2*a(n-6) - 2*a(n-7) for n > 9 initial terms: a(1)=1,
 *            a(2)=7, a(3)=20, a(4)=57, a(5)=156, a(6)=438, a(7)=1220,
 *            a(8)=3398, a(9)=9468
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values: cache[i] holds a(i+1) (1‑based index). */
static seq_t *cache      = NULL;   /* array pointer */
static idx_t  cache_size = 0;       /* number of terms stored */
static idx_t  cache_cap  = 0;       /* allocated capacity */

/* Initial terms for n = 1 .. 9 */
static const seq_t init_terms[9] = {
    1, 7, 20, 57, 156, 438, 1220, 3398, 9468
};

/* Grow the cache array to at least 'needed' capacity. */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    /* Realloc the array and cast to seq_t* so it compiles in C++ */
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr && new_cap > 0)      /* allocation failed */
        abort();

    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1.  Subsequent calls are memoized. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 – return 0 as safe default */
        return (seq_t)0;

    /* Ensure the cache can hold at least n terms. */
    grow_cache(n);

    /* If we already have the value, return it. */
    if (n <= cache_size)
        return cache[n-1];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 9) {
            val = init_terms[i-1];
        } else {
            /* indices: a(i-1)=cache[i-2], a(i-2)=cache[i-3], a(i-3)=cache[i-4],
               a(i-5)=cache[i-6], a(i-6)=cache[i-7], a(i-7)=cache[i-8] */
            val = 2 * cache[i-2]
                + cache[i-3]
                + 3 * cache[i-4]
                + 2 * cache[i-6]
                + 2 * cache[i-7]
                - 2 * cache[i-8];
        }
        cache[i-1] = val;
        cache_size = i;
    }

    return cache[n-1];
}
