/*
 * A132078  Multiply previous term by 6 and reverse.
 * https://oeis.org/A132078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(6 * a(n-1)), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, free */
#include <stdint.h>    /* for uintptr_t, if needed */
#include "../utils/rev_decimal.h"

/* Reverse the decimal digits of a value stored in seq_t. */

/* Compute a(n) for the OEIS sequence A132078.  The sequence is defined
 * recursively: a(1)=1; a(n) = reverse(6 * a(n-1)).  This implementation
 * memoises already computed terms so that calls may arrive in any order.
 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based index; cache[0] unused */
    static idx_t cache_cap = 0;
    static idx_t cached_n = 0;    /* greatest n for which cache[n] is valid */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = (n > 16 ? n : 16);              /* initial capacity */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;                /* allocation failure */
        cache[1] = 1;                               /* a(1) */
        cached_n = 1;
    }

    /* Grow capacity if required. */
    if (n > cache_cap) {
        idx_t new_cap = (n > cache_cap * 2 ? n : cache_cap * 2);
        seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        if (new_cache == NULL) return 0;            /* allocation failure */
        for (idx_t i = 1; i <= cached_n; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t t = cache[i - 1];
            t = t * 6;
            t = rev_decimal(t);
            cache[i] = t;
        }
        cached_n = n;
    }

    return cache[n];
}
