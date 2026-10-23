/*
 * A035057  Numbers k such that 2^k does not contain the digit 1 (probably
 *          finite).
 * https://oeis.org/A035057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   power_has_no_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035057: a(n) = the n-th integer k for which 2^k has no
   decimal digit equal to 1.  This implementation
   uses incremental decimal arithmetic to avoid overflow of 2^k. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/power_has_no_one.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for n == 0, but return 0 */

    /* Static state retained between calls. */
    static idx_t last_k_checked = 0;      /* exponent last processed               */
    static size_t digit_len       = 0;    /* current decimal length of 2^k         */
    static uint8_t *digits        = NULL; /* decimal digits (little‑endian)        */
    static seq_t *cache           = NULL; /* cached sequence values                */
    static size_t cache_size      = 0;    /* number of cached terms                */
    static size_t cache_cap       = 0;    /* allocated capacity of cache           */

    /* Initialise storage for 2^0 == 1. */
    if (digits == NULL) {
        digits       = (uint8_t *)malloc(1);
        digits[0]    = 1;    /* 2^0 = 1 */
        digit_len    = 1;
    }

    /* Initialise cache if necessary. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Compute next power 2^(last_k_checked+1). */
        ++last_k_checked;
        /* Multiply current decimal number by 2. */
        uint16_t carry = 0;
        for (size_t i = 0; i < digit_len; ++i) {
            uint16_t prod = (uint16_t)digits[i] * 2 + carry;
            digits[i]    = (uint8_t)(prod % 10);
            carry        = prod / 10;
        }
        while (carry > 0) {
            if (digit_len == 0 || digit_len >= SIZE_MAX/2) { /* safeguard */
                break;
            }
            /* Grow digits array if necessary. */
            if (digit_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap);
                if (!tmp) return 0;
                digits = tmp;
                cache_cap = new_cap;
            }
            digits[digit_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Check whether current power contains digit 1. */
        if (power_has_no_one(digits, digit_len)) {
            /* Store the exponent k. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_k_checked;
        }
    }

    return cache[n - 1];
}
