/*
 * A233450  Numbers n such that 3*T(n)+1 is a square, where T = A000217.
 * https://oeis.org/A233450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233450
 * Recurrence for n > 5:
 *   a(n) = a(n-1) + 10*a(n-2) - 10*a(n-3) - a(n-4) + a(n-5)
 * Initial values:
 *   a(1)=0, a(2)=1, a(3)=6, a(4)=15, a(5)=64
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), … */
static size_t cache_len = 0;  /* number of terms currently stored */

static const seq_t init_values[5] = {
    (seq_t)0,
    (seq_t)1,
    (seq_t)6,
    (seq_t)15,
    (seq_t)64
};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for indices < 1 */

    /* Return directly for the first five entries. */
    if (n <= 5)
        return init_values[n - 1];

    /* Initialise cache if it hasn't been yet. */
    if (cache == NULL) {
        size_t init_cap = 5;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;      /* out of memory */
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init_values[i];
        cache_len = 5;
    }

    /* Grow the cache to accommodate index n. */
    if (cache_len < (size_t)n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* out of memory */
        cache = tmp;
        /* cache_len remains the number of terms already computed. */
    }

    /* Compute any missing terms up to a(n). */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to term a(i+1) (zero‑based index). */
        seq_t term = cache[i - 1]
                   + ((seq_t)10) * cache[i - 2]
                   + cache[i - 5];
        term -= ((seq_t)10) * cache[i - 3] + cache[i - 4];
        cache[i] = term;
    }

    cache_len = (size_t)n;            /* all terms up to a(n) are now known */
    return cache[n - 1];
}
