/*
 * A021902  Decimal expansion of 1/898.
 * https://oeis.org/A021902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n-th decimal digit of 1/898 (after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* sequence element type and index type are defined by the build system */
seq_t a(idx_t n)
{
    /* static cache for computed digits */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;   /* number of digits stored (max index +1) */
    static idx_t cache_cap  = 0;   /* allocated capacity */
    static idx_t remainder  = 1;   /* remainder after last computed digit */

    /* grow cache if needed */
    if (n >= cache_size) {
        /* determine new capacity */
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
            while (new_cap < needed) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed – unlikely in tests */
            cache  = tmp;
            cache_cap = new_cap;
        }

        /* compute new digits */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* compute current digit and update remainder */
            seq_t digit = (seq_t)((remainder * 10) / 898);
            remainder = (remainder * 10) % 898;
            cache[i] = digit;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
