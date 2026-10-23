/*
 * A044866  Positive integers having the same number of base-10 runs of odd
 *          length as even.
 * https://oeis.org/A044866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sequence of positive integers having the same number of
 *            base‑10 runs of odd length as of even length (computed by
 *            brute‑force search)
 * Range:     u8 n <= 32, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

/* Test whether a value has equal numbers of odd‑length and even‑length runs. */
static bool balanced(seq_t x) {
    size_t len;
    /* buffer sufficient for typical integers */
    static uint8_t digits[128];
    get_digits(x, digits, &len);
    if (len == 0)
        return false;

    unsigned odd = 0, even = 0;
    uint8_t prev = digits[0];
    size_t run = 1;

    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == prev) {
            ++run;
        } else {
            if (run & 1)
                ++odd;
            else
                ++even;
            prev = digits[i];
            run = 1;
        }
    }
    /* final run */
    if (run & 1)
        ++odd;
    else
        ++even;

    return odd == even;
}

seq_t a(idx_t n) {
    /* static cache of found values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* next integer to test */
    static seq_t next_test = 1;

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        if (balanced(next_test)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = (cache_cap) ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_test;
        }
        ++next_test;
    }

    return cache[(size_t)n - 1];
}
