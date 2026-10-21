/*
 * A204367  Number of (n+2) X 7 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) for n > 9 with initial
 *            values a(1) … a(9) as listed in the OEIS entry
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External types provided by the build: */
/* typedef uint64_t idx_t;   (or other width) */
/* typedef … seq_t;          (arbitrary‑precision or native integer) */

seq_t a(idx_t n)
{
    /* a(0) is not defined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Initial terms for 1 ≤ n ≤ 9.  They are the same for all
       build configurations. */
    static const seq_t init_val[9] = {
        2419, 1715, 1042, 1264, 1984,
         3280,  7376, 18776, 48392
    };

    if (n <= 9) return init_val[n - 1];

    /* Memoization cache – grows only once per program run. */
    static seq_t *cache = NULL;      /* array of terms a(1) … a(current) */
    static size_t cache_size = 0;    /* number of terms stored */

    /* First allocation: store the 9 known terms. */
    if (!cache) {
        cache_size = 9;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();        /* allocation failure */
        for (size_t i = 0; i < 9; ++i) cache[i] = init_val[i];
    }

    /* Grow the cache if necessary. */
    if ((size_t)(n - 1) >= cache_size) {
        size_t new_size = (size_t)n;   /* need indices 0 … n‑1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;

        /* Compute all missing terms using the recurrence. */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* i corresponds to a(i+1).  The recurrence uses terms
               with indices i-1, i-4, i-6.  All of those are already
               present because i ≥ cache_size ≥ 9. */
            cache[i] = 3 * cache[i - 1] - 6 * cache[i - 4] + 4 * cache[i - 6];
        }
        cache_size = new_size;
    }

    return cache[n - 1];  /* zero‑based index */
}
