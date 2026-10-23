/*
 * A021658  Decimal expansion of 1/654.
 * https://oeis.org/A021658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point in the expansion of
 *            1/654 (with a(0) being the first digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Lazily compute digits of 1/654 using long division and cache them. */
    static seq_t *cache = NULL;          /* array of computed digits   */
    static size_t cache_cap = 0;         /* allocated size             */
    static size_t cache_len = 0;         /* number of digits stored    */
    static idx_t cur_rem = 1;            /* remainder before next digit */

    /* Ensure the cache has at least n+1 digits. */
    while (cache_len <= n) {
        /* One step of long division: multiply remainder by 10 and
           determine the next decimal digit and new remainder. */
        idx_t prod = cur_rem * 10ULL;
        idx_t digit = prod / 654ULL;
        idx_t new_rem = prod % 654ULL;

        /* Resize the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the digit and advance state. */
        cache[cache_len++] = (seq_t)digit;
        cur_rem = new_rem;
    }

    return cache[n];
}
