/*
 * A021878  Decimal expansion of 1/874.
 * https://oeis.org/A021878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth decimal digit of 1/874 (0‑based)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of computed digits */
    static idx_t cache_len = 0;       /* number of digits stored */
    static idx_t cache_cap = 0;       /* allocated capacity */
    static seq_t rem = 1;             /* remainder after last digit */

    /* need to compute up to digit n, inclusive */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;          /* required length */

        /* grow cache if needed */
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < new_len) new_cap = new_len;
            /* realloc existing cache or allocate new */
            if (cache == NULL) {
                cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            } else {
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            }
            cache_cap = new_cap;
        }

        /* generate missing digits */
        for (idx_t i = cache_len; i <= n; ++i) {
            rem *= 10;
            seq_t digit = rem / 874;
            rem   = rem % 874;
            cache[i] = digit;
        }
        cache_len = new_len;
    }

    return cache[n];
}
