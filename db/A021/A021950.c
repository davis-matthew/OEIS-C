/*
 * A021950  Decimal expansion of 1/946.
 * https://oeis.org/A021950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth digit after the decimal point of 1/946 (OEIS
 *            A021950). 0‑based indexing: a(0) is the first digit after the
 *            decimal point
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the nth decimal digit of 1/946.
   Maintains a static cache of previously computed digits so the
   function works in O(1) amortised time for each call. */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;   /* array of digits */
    static size_t cache_len = 0;  /* number of computed digits */
    static size_t cache_cap = 0;  /* allocated capacity */
    static seq_t rem = 0;         /* current remainder of division */
    static bool initialized = false;

    /* initialise when called first time */
    if (!initialized) {
        rem = (seq_t)1;          /* numerator */
        initialized = true;
    }

    /* extend cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;

        /* increase capacity (double or set to 16) */
        if (need > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }

        /* compute new digits up to n */
        while (cache_len <= n) {
            rem *= (seq_t)10;
            seq_t digit = rem / (seq_t)946;
            rem %= (seq_t)946;
            cache[cache_len++] = digit;
        }
    }

    return cache[(size_t)n];
}
