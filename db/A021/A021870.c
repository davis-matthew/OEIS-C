/*
 * A021870  Decimal expansion of 1/866.
 * https://oeis.org/A021870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the decimal digits of 1/866.
   a(n) = the (n+1)th digit after the decimal point of 1/866,
   with a(0) the first digit following the decimal point. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache of already computed digits (0‑based index) */
static unsigned char *digit_cache = NULL;   /* allocated buffer of digits */
static size_t cache_len   = 0;              /* number of digits stored */
static size_t cache_cap   = 0;              /* allocated capacity */
static unsigned int remainder = 0;          /* current remainder in long division */

/* Return the n‐th digit (0‑based) of 1/866. */
seq_t a(idx_t n)
{
    /* initialise on first call: start with numerator 1 */
    if (cache_len == 0 && digit_cache == NULL) {
        remainder = 1 % 866;        /* 1 is the starting numerator */
    }

    /* If the cache does not cover the requested index, extend it */
    if ((size_t)n >= cache_len) {
        size_t target = (size_t)n;

        /* enlarge the buffer if necessary */
        if (target + 1 > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            if (new_cap <= target + 1) new_cap = target + 1;
            unsigned char *tmp = (unsigned char *)realloc(digit_cache, new_cap);
            digit_cache = tmp;
            cache_cap   = new_cap;
        }

        /* generate missing digits up to target */
        for (size_t i = cache_len; i <= target; ++i) {
            remainder *= 10;
            unsigned char d = remainder / 866;
            remainder     %= 866;
            digit_cache[i] = d;
        }
        cache_len = target + 1;
    }

    /* return the stored digit as seq_t */
    return (seq_t)digit_cache[(size_t)n];
}
