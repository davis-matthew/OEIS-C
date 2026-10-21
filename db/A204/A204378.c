/*
 * A204378  Number of (n+2) X 7 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A204378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5) for n>8 with initial
 *            values: a(1)=182, a(2)=88, a(3)=98, a(4)=111, a(5)=136,
 *            a(6)=176, a(7)=241, a(8)=348
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
#if 0
    /* For safety: handle index zero */
#endif
    /* Cached values are stored with 1‑based indexing. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t max_comp = 0;          /* largest index for which value is computed */

    /* Initial values (index 0 unused). All constants are cast to seq_t. */
    static const seq_t init[9] = {
        (seq_t)0,   /* dummy for index 0 */
        (seq_t)182,
        (seq_t)88,
        (seq_t)98,
        (seq_t)111,
        (seq_t)136,
        (seq_t)176,
        (seq_t)241,
        (seq_t)348
    };

    if (n == 0)
        return (seq_t)0;              /* should never be called for index 0 */

    /* Ensure the cache has room for index n. */
    size_t needed = (size_t)n + 1;    /* +1 because index 0 is unused */

    if (cache == NULL) {
        /* First call: allocate exactly the needed size. */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;         /* out‑of‑memory, unlikely */
        cap = needed;

        /* Store the known initial values. */
        for (idx_t i = 1; i <= 8 && i < (idx_t)cap; ++i)
            cache[i] = init[i];

        /* Compute any remaining terms up to n (only if n>=9). */
        for (idx_t i = 9; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                     - cache[i-4] - cache[i-5];
        }
        max_comp = n;
        return cache[n];
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_comp)
        return cache[n];

    /* Grow the cache if necessary. */
    if (needed > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;         /* out‑of‑memory */
        cache = tmp;
        cap   = new_cap;

        /* Ensure the first eight entries are set to the initial values. */
        for (idx_t i = 1; i <= 8 && i < (idx_t)cap; ++i)
            cache[i] = init[i];
    }

    /* Compute the missing terms only. */
    idx_t start = max_comp + 1;
    if (start < 9)
        start = 9;                    /* recurrence definition starts at 9 */

    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                 - cache[i-4] - cache[i-5];
    }
    max_comp = n;
    return cache[n];
}
