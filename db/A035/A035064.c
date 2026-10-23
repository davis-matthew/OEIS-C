/*
 * A035064  Numbers k such that 2^k does not contain the digit 9 (probably
 *          finite).
 * https://oeis.org/A035064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 2^k does not contain the digit 9.
   Computation: generate the decimal representation of successive 2^k
   by simple multiplication by 2, and test whether any digit equals 9.
   The values a(n) are returned in increasing order of k. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of found exponents; cache[i] holds a(i+1). */
    static seq_t *cache  = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;

    /* Static table of decimal digits of the current power 2^k. */
    static uint8_t *digits = NULL;
    static size_t digits_cap = 0;
    static size_t digits_len = 0;

    /* The next exponent that will be examined. */
    static idx_t last_checked_k = 0;

    /* Initialise structures on first call. */
    if (digits_len == 0) {
        digits_cap = 16;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) abort();
        /* 2^0 == 1 */
        digits[0] = 1;
        digits_len = 1;
    }

    /* Produce enough terms for the requested index. */
    while (cache_size < n) {
        /* Check whether the current 2^k contains a 9. */
        bool has9 = false;
        for (size_t i = 0; i < digits_len; ++i) {
            if (digits[i] == 9) {
                has9 = true;
                break;
            }
        }
        if (!has9) {
            /* Store the exponent (k) in the cache. */
            if (cache_size + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)last_checked_k;
        }

        /* Multiply the decimal digits by 2 to obtain 2^(k+1). */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 2 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = (uint8_t)(prod / 10);
        }
        while (carry > 0) {
            if (digits_len == digits_cap) {
                digits_cap = digits_cap ? digits_cap * 2 : 16;
                uint8_t *tmp = (uint8_t *)realloc(digits, digits_cap * sizeof(uint8_t));
                if (!tmp) abort();
                digits = tmp;
            }
            digits[digits_len++] = carry % 10;
            carry /= 10;
        }

        ++last_checked_k;
    }

    return cache[n - 1];
}
