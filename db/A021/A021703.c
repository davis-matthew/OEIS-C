/*
 * A021703  Decimal expansion of 1/699.
 * https://oeis.org/A021703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th digit of decimal expansion of 1/699, i.e.
 *            a(n) = floor((10^(n+1) * 1) / 699) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *cache      = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* remainder modulo 699 needed to generate next digits.
       It is kept in an unsigned 64‑bit integer to avoid overflow
       when seq_t may be as small as uint8_t. */
    static uint64_t remainder = 1;   /* initial remainder 1 */

    /* Grow the cache if needed */
    while (cache_size <= n) {
        /* compute next digit */
        remainder *= 10ULL;
        seq_t digit = (seq_t)(remainder / 699ULL);
        remainder  = remainder % 699ULL;

        /* ensure capacity */
        if (cache_cap <= cache_size) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – terminate program safely */
                abort(); 
            }
            cache     = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = digit;
    }

    return cache[n];
}
