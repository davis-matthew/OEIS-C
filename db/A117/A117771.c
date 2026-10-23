/*
 * A117771  Numbers k such that the digit sum of 2^k is less than or equal to
 *          k.
 * https://oeis.org/A117771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117771: the n‑th integer k for which the digit sum of 2^k is ≤ k. */
/* The algorithm iterates k from 1 upward, maintaining the decimal
 * representation of 2^k in an array of bytes.  Each step doubles this
 * array and recomputes the digit sum; when the condition holds the value
 * of k is cached.  This works for any concrete integer type seq_t,
 * including arbitrary‑precision integer classes. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache of found k’s. */
    static seq_t *cache = NULL;      /* array of found k's as seq_t */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Static state of the current power of two in decimal digit form. */
    static uint8_t *digits = NULL;   /* least significant digit first */
    static size_t digits_len = 0;   /* number of used digits */
    static size_t digits_cap = 0;   /* allocated capacity */

    /* Next exponent to examine, starting from 0 (2^0 = 1). */
    static idx_t next_k = 0;

    /* Initialize static allocations on first call. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    if (digits_cap == 0) {
        digits_cap = 16;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        if (!digits) abort();
        digits[0] = 1;  /* 2^0 = 1 */
        digits_len = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Advance to the next exponent. */
        ++next_k;

        /* Double the current value stored in decimal array. */
        int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            int prod = digits[i] * 2 + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0) {
            if (digits_len >= digits_cap) {
                size_t new_cap = digits_cap ? digits_cap * 2 : 16;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) abort();
                digits = tmp;
                digits_cap = new_cap;
            }
            digits[digits_len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Compute the digit sum of the current 2^next_k. */
        seq_t ds = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            ds += (seq_t)digits[i];
        }

        /* If the condition holds, store the exponent. */
        if (ds <= (seq_t)next_k) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
    }

    return cache[n - 1];
}
