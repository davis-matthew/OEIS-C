/*
 * A176621  a(n) = 2 + Sum_{k=0..n-1} A176513(4*k+1).
 * https://oeis.org/A176621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 3, a(2) = 4, a(3) = 11, a(4) = 40 a(n+5) =
 *            2*a(n+4) + 7*a(n+3) + 9*a(n+2) - 2*a(n+1) + a(n) - 32 (valid for
 *            all n >= 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ensure capacity of the cache array */
static void
ensure_capacity(size_t needed, seq_t **cache, size_t *cap)
{
    if (*cap < needed) {
        size_t newcap = *cap ? *cap * 2 : 8;
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t *)realloc(*cache, newcap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out‑of‑memory */
        *cache = tmp;
        *cap   = newcap;
    }
}

seq_t
a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size */
    static size_t cooked = 0;         /* highest index computed (inclusive) */

    /* initialise cache with base values on first call */
    if (!cache) {
        ensure_capacity(6, &cache, &cap);   /* store first 6 terms */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)40;
        cache[5] = (seq_t)157;
        cooked   = 5;
    }

    /* compute up to the requested index if needed */
    if ((idx_t)n > cooked) {
        ensure_capacity((size_t)n + 1, &cache, &cap);
        for (idx_t i = cooked + 1; i <= n; ++i) {
            /* recurrence for i >= 5:
               a(i) = 2*a(i-1) + 7*a(i-2) + 9*a(i-3)
                      - 2*a(i-4) + a(i-5) - 32 */
            cache[i] =
                2 * cache[i-1] +
                7 * cache[i-2] +
                9 * cache[i-3] -
                2 * cache[i-4] +
                cache[i-5] -
                (seq_t)32;
        }
        cooked = n;
    }

    return cache[n];
}
