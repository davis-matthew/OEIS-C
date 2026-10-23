/*
 * A015009  q-factorial numbers for q=10.
 * https://oeis.org/A015009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1, a(n) = a(n-1) * (10^n - 1) / 9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static size_t cap   = 0;      /* allocated size of cache */
    static size_t size  = 0;      /* number of values currently stored */

    /* static variable that always holds 10^(size‑1) */
    static seq_t power10 = 1;     /* 10^0 = 1 when cache is empty */

    /* initialise cache with a(0)=1 */
    if (size == 0) {
        if (cap == 0) {
            cap   = 4;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        size = 1;
        power10 = 1;          /* 10^(size-1) = 10^0 */
    }

    /* if we already know a(n), return it immediately */
    if (n < size)
        return cache[n];

    /* compute successive terms until we reach n */
    for (idx_t k = size; k <= n; ++k) {
        /* 10^k: multiply previous power by 10 */
        power10 *= (seq_t)10;               /* now 10^k */
        seq_t term = power10 - (seq_t)1;     /* 10^k - 1 */

        /* a(k) = a(k‑1) * term / 9 */
        seq_t product = cache[size - 1] * term;
        seq_t next    = product / (seq_t)9; /* division is exact */

        /* grow cache if necessary */
        if (size == cap) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed, abort */
                /* The return value is undefined in this situation. */
                return (seq_t)0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        cache[size] = next;
        ++size;
    }

    return cache[n];
}
