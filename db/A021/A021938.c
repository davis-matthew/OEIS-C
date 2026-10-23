/*
 * A021938  Decimal expansion of 1/934.
 * https://oeis.org/A021938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021938 – Decimal expansion of 1/934
 *  a(0) = 0 (integer part), a(n) gives the n‑th digit of the decimal
 *  representation of 1/934, counting the integer part as the 0‑th digit.
 *  The value is produced by successive long‑division steps:
 *      r ← 1
 *      repeat:
 *          r ← r·10
 *          digit ← r div 934
 *          r ← r mod 934
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits and the current remainder */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0;
    static idx_t cache_cap   = 0;
    static idx_t rem         = 1;     /* numerator of 1/934 before any digit */

    const idx_t DIV = 934;

    /* grow cache to contain at least n+1 entries (digit indices 0..n) */
    if (n >= cache_len) {
        idx_t target = n + 1;

        if (target > cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap : 128;
            while (new_cap < target) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);        /* allocation failure */
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* generate missing digits */
        for (idx_t i = cache_len; i < target; ++i) {
            rem *= 10;
            seq_t digit = (seq_t)(rem / DIV);
            rem %= DIV;
            cache[i] = digit;
        }
        cache_len = target;
    }

    return cache[n];
}
