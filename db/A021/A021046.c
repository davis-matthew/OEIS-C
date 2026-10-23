/*
 * A021046  Decimal expansion of 1/42.
 * https://oeis.org/A021046
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

/* Implements the decimal expansion of 1/42: a(n) is the nth digit after
   the decimal point of 1/42, with a(0)=0.  The function generates the
   digits on demand and caches them to allow repeated, unordered calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *cache = NULL;      /* array of digits a(0)...a(len-1) */
    static idx_t len = 0;            /* number of digits currently stored */
    static idx_t cap = 0;            /* allocated capacity of cache */
    static seq_t remainder = 1;      /* current remainder in long division */

    /* Extend the cache until it contains the requested index */
    while (len <= n) {
        if (len == cap) {                         /* grow storage */
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);                    /* out of memory */
            cache = tmp;
            cap = new_cap;
        }
        /* next digit by long division: floor((remainder*10)/42) */
        seq_t digit = (remainder * 10) / 42;
        /* update remainder for the next step */
        remainder = (remainder * 10) % 42;
        cache[len++] = digit;
    }
    return cache[n];
}
