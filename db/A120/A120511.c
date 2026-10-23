/*
 * A120511  a(n) = min{j>0 : A006949(j) = n}.
 * https://oeis.org/A120511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(ceil(n/2)) + n, a(1) = 1. This computes OEIS A120511
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out‑of‑range, not used for OEIS offset */

    static seq_t *cache = NULL;   /* 1‑based cache of computed a values */
    static idx_t cache_size = 0;  /* number of allocated elements (size+1) */

    if (n >= cache_size) {
        /* Need to grow cache to hold indices up to n */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failed – unlikely for tests */

        cache = tmp;
        /* Compute new entries in order; previous entries are already valid */
        for (idx_t i = cache_size; i <= new_size; ++i) {
            if (i == 1)
                cache[1] = (seq_t)1;
            else
                cache[i] = cache[(i + 1) / 2] + (seq_t)i;
        }
        cache_size = new_size + 1;
    }
    return cache[n];
}
