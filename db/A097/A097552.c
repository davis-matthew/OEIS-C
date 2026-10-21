/*
 * A097552  Number of positive words of length n in the monoid Br_5 of
 *          positive braids on 6 strands.
 * https://oeis.org/A097552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=20, a(3)=67, a(4)=209, a(5)=630, a(6)=1873;
 *            for n≥7 a(n)=5*a(n-1)-8*a(n-2)+7*a(n-3)-4*a(n-4)+a(n-5)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoised array of a(n) */
static idx_t cache_cap = 0;     /* current array capacity */

/* Pre-initialise the first 7 terms as needed */
static void init_cache(idx_t n)
{
    const seq_t init_vals[7] = {(seq_t)1,(seq_t)5,(seq_t)20,(seq_t)67,
                                (seq_t)209,(seq_t)630,(seq_t)1873};
    if (cache == NULL) {
        /* Allocate space for at least n+1 entries */
        cache_cap = (n >= 6 ? n+1 : 7);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 7 && i <= n; ++i)
            cache[i] = init_vals[i];
    } else if (n >= cache_cap) {
        /* Grow the cache to cover up to n */
        idx_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        /* Initialise any new slots (they will be filled by the loop below) */
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 7) {
        const seq_t init_vals[7] = {(seq_t)1,(seq_t)5,(seq_t)20,(seq_t)67,
                                    (seq_t)209,(seq_t)630,(seq_t)1873};
        return init_vals[n];
    }

    init_cache(n);

    /* Compute any missing terms up to n */
    for (idx_t i = 7; i <= n; ++i) {
        if (i < 7) continue;     /* already initialised */

        /* Use the recurrence: a(i) = 5*a(i-1) - 8*a(i-2) + 7*a(i-3) - 4*a(i-4) + a(i-5) */
        seq_t val = 0;
        val += cache[i-1] * (seq_t)5;
        val -= cache[i-2] * (seq_t)8;
        val += cache[i-3] * (seq_t)7;
        val -= cache[i-4] * (seq_t)4;
        val += cache[i-5];
        cache[i] = val;
    }

    return cache[n];
}
