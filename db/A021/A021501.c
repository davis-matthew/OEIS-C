/*
 * A021501  Decimal expansion of 1/497.
 * https://oeis.org/A021501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit (0‑indexed) after the decimal point in the
 *            expansion of 1/497. Example: a(0)=0, a(1)=0, a(2)=2, …
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of digits already computed */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of digits stored */
    static size_t cache_cap  = 0;   /* allocated capacity of cache   */
    static seq_t rem = 1;           /* remainder before computing the next digit */

    /* ensure enough space for n+1 digits */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – return 0 as a fallback */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing digits up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        rem *= (seq_t)10;               /* bring down next decimal digit   */
        seq_t digit = rem / (seq_t)497; /* quotient gives the digit        */
        rem = rem % (seq_t)497;         /* store new remainder             */
        cache[i] = digit;               /* cache the digit                 */
    }
    cache_size = needed;                /* update number of cached digits  */

    return cache[(size_t)n];
}
