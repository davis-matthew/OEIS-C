/*
 * A218200  Hilltop maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or vertical neighbor in a random 0..2 n X 2 array.
 * https://oeis.org/A218200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)+3*a(n-3)+2*a(n-4)+2*a(n-5)
 *            -a(n-6)-a(n-7)-a(n-8)-a(n-9), with offset 1 Initial terms: 3,
 *            15, 59, 233, 929, 3697, 14719, 58599, 233291, …
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to allow memoised, arbitrary‑order calls */
    static seq_t *cache = NULL;          /* cache[1 .. cache_cap-1] are usable */
    static size_t cache_size = 0;        /* highest computed index (0 if none) */
    static size_t cache_cap  = 0;        /* allocated capacity (indices 0 .. cache_cap-1) */

    /* Initial nine terms (offset 1) */
    static const seq_t init[9] = {
        (seq_t)3,     (seq_t)15,   (seq_t)59,
        (seq_t)233,   (seq_t)929,  (seq_t)3697,
        (seq_t)14719, (seq_t)58599,(seq_t)233291
    };

    /* If the requested term is among the initial ones, return it immediately */
    if (n >= 1 && n <= 9)
        return init[(size_t)n - 1];

    /* Ensure the cache is large enough for index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;          /* indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Populate cache[1..9] on first use */
    if (cache_size < 9) {
        for (size_t i = cache_size + 1; i <= 9; ++i)
            cache[i] = init[i - 1];
        cache_size = 9;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = (idx_t)(cache_size + 1); i <= n; ++i) {
        seq_t t = cache[i - 1] * (seq_t)3
                + cache[i - 2] * (seq_t)3
                + cache[i - 3] * (seq_t)3
                + cache[i - 4] * (seq_t)2
                + cache[i - 5] * (seq_t)2;
        t -= cache[i - 6];
        t -= cache[i - 7];
        t -= cache[i - 8];
        t -= cache[i - 9];
        cache[(size_t)i] = t;
    }
    cache_size = (size_t)n;

    return cache[(size_t)n];
}
