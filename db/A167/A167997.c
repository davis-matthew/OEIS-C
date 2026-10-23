/*
 * A167997  Numbers k with property that first digit of 4*k = last digit of k.
 * https://oeis.org/A167997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167997:
   numbers k such that the first decimal digit of 4*k equals the last decimal digit of k.
   The function searches sequentially, caching found terms. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* cached terms (0‑based) */
    static size_t cache_cap = 0;       /* allocated capacity */
    static size_t cache_len = 0;       /* number of stored terms */
    static idx_t  last_k = 1;          /* next candidate to test */

    if (n == 0) return 0;              /* undefined for index 0 */

    /* Continue searching until we have n terms */
    while (cache_len < (size_t)n)
    {
        seq_t k = (seq_t)last_k;
        seq_t fourk = (seq_t)4 * k;

        /* compute first digit of fourk */
        seq_t f = fourk;
        while (f >= 10) f /= 10;

        seq_t last_digit = k % 10;
        if (f == last_digit)
        {
            /* grow the cache if needed */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++last_k;
    }

    return cache[n - 1];                /* n is 1‑based, array is 0‑based */
}
