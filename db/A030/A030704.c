/*
 * A030704  Numbers k such that the decimal expansion of 8^k contains no zeros
 *          (probably finite).
 * https://oeis.org/A030704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   mul_by8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that 8^k contains no decimal zero digits (search) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_by8.h"

/* Helper: ensure the digit array has room for at least 'need' digits */
static void ensure_digits_capacity(size_t need)
{
    static uint8_t *digits = NULL;
    static size_t cap = 0;
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    void *p = realloc(digits, new_cap);
    if (!p)
        abort(); /* fatal: memory exhausted */
    digits = (uint8_t *)p;
    cap = new_cap;
}

/* Does the decimal number currently stored in 'digits' contain a zero? */
static bool has_zero(const uint8_t *digits, size_t len)
{
    for (size_t i = 0; i < len; ++i)
        if (digits[i] == 0)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safety */

    /* Static memoisation structures */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz = 0;
    static uint8_t *digits = NULL;
    static size_t digits_len = 0;
    static idx_t cur_exp = 0; /* exponent of the current digits value */

    /* Initialise on first call */
    if (!cache)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort(); /* fatal */

        ensure_digits_capacity(64);
        digits = (uint8_t *)malloc(64);
        if (!digits)
            abort();
        digits[0] = 1;          /* 8^0 = 1 */
        digits_len = 1;
        cur_exp = 0;

        /* first term is k = 0 */
        cache[0] = (seq_t)0;
        cache_sz = 1;
    }

    /* Extend the cache until we have at least n terms */
    while (cache_sz < (size_t)n)
    {
        /* Compute next power of 8 */
        mul_by8(digits, &digits_len);
        cur_exp++; /* now digits represents 8^cur_exp */

        if (!has_zero(digits, digits_len))
        {
            /* Ensure cache capacity */
            if (cache_sz == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!p)
                    abort();
                cache = p;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)cur_exp;
        }
    }

    return cache[n - 1];
}
