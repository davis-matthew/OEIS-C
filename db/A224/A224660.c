/*
 * A224660  Number of n X n 0..3 matrices with each 2 X 2 subblock idempotent.
 * https://oeis.org/A224660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 6*a(n-1) -12*a(n-2) +5*a(n-3) +12*a(n-4) -12*a(n-5)
 *            -3*a(n-6) +6*a(n-7) -a(n-9) for n > 10. Initial values (a(1) ..
 *            a(10)): 4, 16, 72, 142, 332, 668, 1356, 2634, 5046, 9480
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(idx_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static const seq_t base[10] = {4, 16, 72, 142, 332, 668, 1356, 2634, 5046, 9480};

    if (n <= 0) return 0;  /* undefined for n<1, return 0 */

    if ((idx_t)cache_len >= n) return cache[n-1];

    ensure_capacity(n, &cache_cap, (void **)&cache, sizeof(seq_t));

    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i <= 10)
        {
            cache[i-1] = base[i-1];
        }
        else
        {
            cache[i-1] =
                (seq_t)6  * cache[i-2]          /* 6*a(i-1) */
              - (seq_t)12 * cache[i-3]          /* -12*a(i-2) */
              + (seq_t)5  * cache[i-4]          /* +5*a(i-3) */
              + (seq_t)12 * cache[i-5]          /* +12*a(i-4) */
              - (seq_t)12 * cache[i-6]          /* -12*a(i-5) */
              - (seq_t)3  * cache[i-7]          /* -3*a(i-6) */
              + (seq_t)6  * cache[i-8]          /* +6*a(i-7) */
              -          cache[i-10];           /* -a(i-9) */
        }
    }

    cache_len = (size_t)n;
    return cache[n-1];
}
