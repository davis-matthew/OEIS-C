/*
 * A037362  Numbers whose base-5 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate increasing integers, keeping those whose decimal
   and base‑5 digits have no common digit. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Test whether x has no digit in common between its decimal and base‑5
   representations.  In decimal digits 0–4 disqualify a base‑5 digit
   of the same value.  All other digits are irrelevant. */
static bool passes(idx_t x)
{
    bool dec_digit[5] = {false, false, false, false, false};
    idx_t y = x;

    /* Record decimal digits 0–4.  For 0, its decimal representation
       is a single digit 0. */
    if (y == 0)
        dec_digit[0] = true;
    while (y > 0) {
        uint8_t d = (uint8_t)(y % 10);
        if (d <= 4)
            dec_digit[d] = true;
        y /= 10;
    }

    /* Check the base‑5 digits.  For 0 the representation is a single
       digit 0. */
    y = x;
    if (y == 0) {
        if (dec_digit[0])
            return false;
        return true;
    }
    while (y > 0) {
        uint8_t d = (uint8_t)(y % 5);
        if (dec_digit[d])
            return false;
        y /= 5;
    }
    return true;
}

/* Cache of already found terms, stored as uint64_t to keep C/C++ compatibility. */
static uint64_t *cache = NULL;   /* cache array (values of the sequence) */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* current allocated capacity */
static idx_t next_test = 1;      /* next integer to test for inclusion */

/* Ensure the cache has at least 'need' slots, grow if necessary. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < need)
        new_cap = need;

    /* realloc returns void* in C; cast it for C++ compatibility. */
    uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
    if (!tmp && new_cap > 0) {
        /* allocation failed; terminate (sequence not computable further). */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; return 0 for safety. */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Skip numbers that do not satisfy the condition. */
        while (!passes(next_test))
            ++next_test;

        /* Store the found term (as uint64_t). */
        grow_cache(cache_len + 1);
        cache[cache_len++] = (uint64_t)next_test;

        ++next_test;   /* advance to the next candidate. */
    }

    /* Convert the cached integer to seq_t and return. */
    return (seq_t)cache[n - 1];
}
