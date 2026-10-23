/*
 * A116717  Number of permutations of length n which avoid the patterns 231,
 *          1423, 3214.
 * https://oeis.org/A116717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A116717: Number of permutations of length n avoiding 231, 1423, 3214.
   Recurrence: a(1)=1, a(2)=2, a(3)=5, a(4)=12, a(5)=26,
   a(n)=4*a(n‑1)-5*a(n‑2)+a(n‑3)+2*a(n‑4)-a(n‑5)   for n>5. */
#include <stdlib.h>

static seq_t *cache = NULL;        /* cache[1]..cache[cache_size] computed */
static size_t cache_size = 0;

/* Compute a(n). Works for any seq_t (built‑in unsigned or arbitrary precision). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0ULL;                 /* behaviour for n<1 is irrelevant */

    /* Expand cache if needed */
    if (!cache || cache_size < (size_t)n) {
        size_t new_size = (size_t)n + 1;          /* indices 0..n, 0 unused */
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            return 0ULL;               /* malloc failure – return 0 as fallback */
        cache = new_cache;

        /* Initialise new entries to 0 */
        for (size_t i = cache_size + 1; i < new_size; ++i)
            cache[i] = 0;

        /* Set initial known values */
        if (cache_size < 1 && new_size > 1) cache[1] = 1;
        if (cache_size < 2 && new_size > 2) cache[2] = 2;
        if (cache_size < 3 && new_size > 3) cache[3] = 5;
        if (cache_size < 4 && new_size > 4) cache[4] = 12;
        if (cache_size < 5 && new_size > 5) cache[5] = 26;

        /* Compute remaining terms using the recurrence */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            if (i <= 5)                /* already set */
                continue;
            cache[i] = (seq_t)4 * cache[i-1]
                     - (seq_t)5 * cache[i-2]
                     + cache[i-3]
                     + (seq_t)2 * cache[i-4]
                     - cache[i-5];
        }
        cache_size = (size_t)n;
    }

    return cache[n];
}
