/*
 * A120192  a(1)=7; a(n)=floor((55+sum(a(1) to a(n-1)))/7).
 * https://oeis.org/A120192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; for n>1: a(n)=floor((55+sum_{i=1}^{n-1} a(i))/7)
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* offset is 1; undefined below */

    static seq_t *cache = NULL;  /* memoisation array, 1‑based */
    static idx_t last = 0;       /* largest n for which cache[n] is valid */
    static seq_t sum = 0;        /* sum of a(1)..a(last) */
    static size_t cap = 0;       /* allocated size of cache array */

    if (n <= last)
        return cache[n];

    /* Ensure the array is large enough (size n+1 to access index n). */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();                /* memory allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)7;
            sum = cache[1];
        } else {
            cache[i] = (sum + (seq_t)55) / (seq_t)7;
            sum += cache[i];
        }
    }

    last = n;
    return cache[n];
}
