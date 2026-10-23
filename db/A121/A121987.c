/*
 * A121987  a(n + 4) = (n + 2)*(a(n + 3) - a(n) + 1) for n > 3, a(0) = a(1) =
 *          a(2) = a(3) = 1.
 * https://oeis.org/A121987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = (n+2)·(a(n+3) – a(n) + 1) for n ≥ 0 Initial values:
 *            a(0) = a(1) = a(2) = a(3) = 1 Sequence is defined for all n ≥ 0
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of terms currently cached (≥0) */
    idx_t old_size = cache_size;          /* cache size before potential growth */

    /* Grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 4) new_size = 4;     /* need space for the 4 initial terms */
        cache = (seq_t *)realloc(cache,
                                  sizeof(seq_t) * (size_t)new_size);
        cache_size = new_size;
    }

    /* Ensure the first four terms are the initial ones */
    for (idx_t i = 0; i < 4 && i < cache_size; ++i)
        cache[i] = (seq_t)1;

    /* Compute any missing terms up to n */
    idx_t start = old_size;
    if (start < 4) start = 4;                /* computation only needed from j ≥ 4 */

    for (idx_t i = start; i <= n; ++i) {
        /* Using the recurrence rewritten for i = n+4:
           a(i) = (i-2)·(a(i-1) – a(i-4) + 1)  for i ≥ 4   */
        seq_t diff = cache[i - 1] - cache[i - 4] + (seq_t)1;
        cache[i] = (seq_t)(i - 2) * diff;
    }

    return cache[n];
}
