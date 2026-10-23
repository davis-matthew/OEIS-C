/*
 * A119667  Squares that contain multi-digit prime substrings.
 * https://oeis.org/A119667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 159, u32 n <= 570, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A119667: Squares that contain a multi‑digit prime substring
   The function generates squares in increasing order and keeps those
   that contain at least one decimal substring of length ≥ 2 that is
   prime.  The result is memoised so that any order of calls works. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_digits.h"
#include "../utils/is_prime_big.h"

/* Return true if the decimal number x contains a prime substring of
   length at least 2.  get_digits gives digits least significant first. */
static bool has_prime_substring(seq_t x)
{
    /* A buffer big enough for any seq_t (up to 128 bits). */
    uint8_t digits[32];
    size_t len = 0;

    get_digits(x, digits, &len);
    if (len < 2)
        return false;   /* cannot contain a substring of length ≥ 2 */

    /* Reverse digits to have most significant digit first. */
    uint8_t rev[32];
    for (size_t i = 0; i < len; ++i)
        rev[i] = digits[len - 1 - i];

    for (size_t i = 0; i + 1 < len; ++i) {
        seq_t val = 0;
        for (size_t j = i; j < len; ++j) {
            val = val * 10 + rev[j];
            if (j - i + 1 >= 2) {
                if (is_prime_big(val))
                    return true;
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of found squares. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_root = 1;   /* the next integer to square */

    /* Initialise cache on first call. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate squares until we have n+1 valid terms. */
    while (cache_len <= n) {
        idx_t r = next_root++;
        seq_t sq = (seq_t)r * (seq_t)r;
        if (has_prime_substring(sq)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = sq;
        }
    }

    return cache[n];
}
