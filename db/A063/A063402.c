/*
 * A063402  a(0)=0; a(1)=1; a(2)=2; a(n)= a(n-1) + a(n-2)*a(n-3).
 * https://oeis.org/A063402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2)*a(n-3), a(0)=0, a(1)=1, a(2)=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *values  = NULL;
    static idx_t cache_size = 0;   /* number of values that are already computed */
    static idx_t alloc_size = 0;   /* current allocated array size */

    /* if we need values up to n, grow the cache accordingly */
    if (n >= cache_size) {
        idx_t new_needed = n + 1;
        if (new_needed < 3) new_needed = 3;          /* we always keep space for a(0..2) */

        /* grow the array if necessary */
        if (new_needed > alloc_size) {
            seq_t *tmp = (seq_t *)realloc(values, new_needed * sizeof(seq_t));
            if (!tmp) return 0;                      /* allocation failure – unlikely in tests */
            values    = tmp;
            alloc_size = new_needed;
        }

        /* initialise the first three terms once */
        if (cache_size == 0) {
            values[0] = 0;
            values[1] = 1;
            values[2] = 2;
            cache_size = 3;
        } else if (cache_size == 1) {              /* only a(0) was known */
            values[1] = 1;
            values[2] = 2;
            cache_size = 3;
        } else if (cache_size == 2) {              /* only a(0),a(1) known */
            values[2] = 2;
            cache_size = 3;
        }

        /* compute the missing terms in order */
        for (idx_t i = cache_size; i <= n; ++i)
            values[i] = values[i-1] + values[i-2] * values[i-3];

        cache_size = n + 1;     /* now values[0..n] are valid */
    }

    return values[n];
}
