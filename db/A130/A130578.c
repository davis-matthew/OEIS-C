/*
 * A130578  Number of different possible rows (or columns) in an n X n
 *          crossword puzzle.
 * https://oeis.org/A130578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=0, a(3)=1, a(4)=3
   a(n+4) = 2*a(n+3) - a(n+2) + a(n) + 1   (n ≥ 1) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* cache[1..] holds values */
    static size_t cache_size = 0;    /* current allocated size (index 0 unused) */
    static size_t last_computed = 4; /* highest index already computed */
    static unsigned init = 0;        /* initialisation flag */

    /* Initialise cache once */
    if (!init) {
        cache_size = 5;                  /* indices 0..4 needed for recurrence */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* cannot happen in tests */

        /* set initial values */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)3;
        init = 1;
    }

    /* Handle small arguments directly */
    if ((size_t)n <= 4) return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 64;
        while (new_size <= (size_t)n) new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure unlikely in tests */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* a(i) = 2*a(i-1) - a(i-2) + a(i-4) + 1 */
        seq_t val = cache[i - 1];
        val += val;                       /* 2*a(i-1) */
        val -= cache[i - 2];              /* - a(i-2) */
        val += cache[i - 4];              /* + a(i-4) */
        val += (seq_t)1;                  /* + 1 */
        cache[i] = val;
    }

    last_computed = (size_t)n;
    return cache[n];
}
