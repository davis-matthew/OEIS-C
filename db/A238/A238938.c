/*
 * A238938  Powers of 2 without the digit '0' in their decimal expansion.
 * https://oeis.org/A238938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 21, u64 n <= 30, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A238938: powers of 2 whose decimal
   expansion contains no digit 0.  The function searches for exponents
   k such that 2^k has no zero, stores those values in a cache, and
   returns the n‑th cached value. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /*--- Persistent cache of discovered terms --------------------------------*/
    static seq_t *cache = NULL;        /* array of 2^k values with no zero    */
    static size_t cache_cap = 0;       /* allocated size of cache            */
    static size_t cache_size = 0;      /* number of terms stored             */

    /*--- Persistent state for the next exponent to be tested ----------------*/
    static seq_t current_val = (seq_t)1;   /* 2^last_exp                       */
    static uint8_t *digits = NULL;        /* decimal digits of current_val, LSb first */
    static size_t digits_cap = 0;          /* allocated size of digits          */
    static size_t digits_len = 0;          /* number of used digits in array    */
    static idx_t last_exp = 0;             /* exponent of current_val           */

    /*--- One time initialization --------------------------------------------*/
    if (cache == NULL) {
        /* cache for terms */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_size = 1;
        cache[0] = (seq_t)1;            /* a(1) = 1 = 2^0 */

        /* decimal representation of 2^0 */
        digits_cap = 16;
        digits = (uint8_t*)malloc(digits_cap * sizeof(uint8_t));
        digits_len = 1;
        digits[0] = 1;
        current_val = (seq_t)1;
        last_exp = 0;
    }

    /*--- If the requested index is already known, return it ----------------*/
    if ((idx_t)n <= cache_size)
        return cache[n - 1];

    /*--- Grow cache array if needed ----------------------------------------*/
    if (cache_cap < cache_size + 1) {
        cache_cap *= 2;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /*--- Generate terms until we have the n-th one --------------------------*/
    while (cache_size < (size_t)n) {
        /* 1. Increment exponent */
        last_exp++;

        /* 2. Multiply current value by 2 (big‑int arithmetic) */
        current_val = current_val * (seq_t)2;

        /* 3. Multiply the decimal digits by 2 */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint8_t temp = digits[i] * 2 + carry;
            digits[i] = temp % 10;
            carry = temp / 10;
        }
        while (carry) {
            if (digits_len == digits_cap) {
                digits_cap <<= 1;
                digits = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
            }
            digits[digits_len++] = carry % 10;
            carry /= 10;
        }

        /* 4. Check if any decimal digit is 0 */
        bool has_zero = false;
        for (size_t i = 0; i < digits_len; ++i) {
            if (digits[i] == 0) {
                has_zero = true;
                break;
            }
        }

        /* 5. If no zero, cache the term */
        if (!has_zero) {
            cache[cache_size++] = current_val;
        }
    }

    return cache[n - 1];
}
