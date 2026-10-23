/*
 * A137280  a(n) = 3*a(n-1) + 7*a(n-2), with a(1) = 1, a(2) = 10.
 * https://oeis.org/A137280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2), with a(1) = 1, a(2) = 10
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; any call with n == 0 returns 0. */
    if (n == 0)
        return (seq_t)0;

    /* Persistent cache of generated terms. */
    static seq_t *buf = NULL;
    static idx_t  max_cached = 0;   /* largest index that is cached */

    /* Ensure the cache is allocated and large enough. */
    if (buf == NULL) {
        /* At least two elements are needed to start the recurrence. */
        idx_t cap = (n > 2 ? n : 2);
        buf = (seq_t *)malloc((size_t)(cap + 1) * sizeof(seq_t));
        if (buf == NULL)
            abort();                  /* Out of memory */

        /* Initialise first two terms. */
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)10;

        /* Compute any additional terms that might be needed on the first call. */
        for (idx_t i = 3; i <= cap; ++i)
            buf[i] = (seq_t)(3 * buf[i-1] + 7 * buf[i-2]);

        max_cached = cap;
    } else if (n > max_cached) {
        /* Reallocate cache to accommodate the requested index. */
        idx_t new_cap = n;
        if (new_cap < max_cached)
            new_cap = max_cached;

        seq_t *new_buf = (seq_t *)realloc(buf,
                            (size_t)(new_cap + 1) * sizeof(seq_t));
        if (new_buf == NULL)
            abort();                  /* Out of memory */
        buf = new_buf;

        /* Compute terms that were not previously cached. */
        for (idx_t i = max_cached + 1; i <= n; ++i)
            buf[i] = (seq_t)(3 * buf[i-1] + 7 * buf[i-2]);

        max_cached = n;
    }

    return buf[n];
}
