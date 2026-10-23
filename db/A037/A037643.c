/*
 * A037643  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1.
 * https://oeis.org/A037643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A037643
 *  A037643: Base 8 digits are, in order, the first n terms of the
 *  periodic sequence with initial period 2,3,1.
 *  a(1) = 2
 *  a(n) = 8 * a(n-1) + d(n)
 *  where d(n) = 2,3,1 repeating.
 *  Implementation uses a static cache so calls may be in any order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache = NULL;      /* array of seq_t */
    static idx_t cache_len = 0;      /* number of terms stored (cache_len == 0 means not yet allocated) */
    static idx_t cache_cap = 0;      /* current capacity of cache */

    /* First call: allocate a small initial buffer and compute a(1). */
    if (cache == NULL) {
        cache_cap = 16;                     /* arbitrary small start */
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)cache_cap);
        if (cache == NULL) return (seq_t)0;  /* allocation failure – caller will get 0 */
        cache[0] = (seq_t)2;                /* a(1) = 2 */
        cache_len = 1;
    }

    /* Expand cache until it contains the requested index n. */
    while (cache_len < n) {
        /* Grow capacity if necessary. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap << 1;  /* double */
            seq_t *tmp = (seq_t *)realloc(cache,
                                           sizeof(seq_t) * (size_t)new_cap);
            if (tmp == NULL) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Next index (1‑based). */
        idx_t idx = cache_len + 1;

        /* Determine the next digit in the 2,3,1 cycle. */
        seq_t digit;
        switch (idx % 3) {
            case 1: digit = (seq_t)2; break;
            case 2: digit = (seq_t)3; break;
            default: digit = (seq_t)1; /* idx % 3 == 0 */
        }

        /* Compute the next term. */
        seq_t prev = cache[cache_len - 1];
        seq_t val = prev * (seq_t)8 + digit;
        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n - 1];   /* cache is 0‑based but indices are 1‑based */
}
