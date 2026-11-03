/*
 * A061729  Start with 0; to get next term reverse digits and add 1 to each
 *          digit (9's get replaced by 10's).
 * https://oeis.org/A061729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 27, u32 n <= 37, u64 n <= 46, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 97 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061729
 * a(0) = 0
 * a(n)  = reverse of a(n‑1) with each digit increased by 1
 *        (digit 9 becomes the two digits 10).
 *
 * Implementation uses a static cache so that calls to a(idx_t n) are
 * O(n) total (amortised). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include <string.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Convert a seq_t number into its decimal digits (most‑significant digit first)
   and return the length.  The buffer must be at least 128 bytes (enough for
   seq_t values up to 128 decimal digits). */
static void seq_to_digits(seq_t val, uint8_t *buf, size_t *len)
{
    if (val == 0) {
        buf[0] = 0;
        *len   = 1;
        return;
    }
    size_t i = 0;
    while (val != 0) {
        uint8_t d = (uint8_t)(val % 10);
        buf[i++] = d;
        val /= 10;
    }
    /* reverse to most significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        uint8_t t = buf[j];
        buf[j]    = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    *len = i;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Initialise cache with the first term a(0)=0 if needed. */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return (seq_t)0;          /* out of memory - give up */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Compute terms up to n inclusive. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        const seq_t prev = cache[i - 1];

        /* Number of decimal digits of prev. */
        size_t dlen = (size_t)num_digits(prev);

        /* Extract the digits of prev (most‑significant first). */
        uint8_t *digits = (uint8_t *)malloc(dlen * sizeof(uint8_t));
        if (!digits) return (seq_t)0;  /* allocation failure */

        size_t actual = dlen;
        seq_to_digits(prev, digits, &actual);   /* actual == dlen for valid integer */

        /* Build the new decimal representation. */
        size_t outmax = 2 * dlen + 1;          /* each old digit gives at most two new ones */
        char *out = (char *)malloc(outmax);
        if (!out) {
            free(digits);
            return (seq_t)0;
        }

        size_t pos = 0;
        for (size_t j = 0; j < dlen; ++j) {
            uint8_t d = digits[dlen - 1 - j];   /* reverse order */
            if (d == 9) {
                out[pos++] = '1';
                out[pos++] = '0';
            } else {
                out[pos++] = (char)('0' + d + 1);
            }
        }
        out[pos] = '\0';

        /* Convert the string back to a seq_t. */
        seq_t next = seq_from_str(out);

        /* Grow the cache if necessary. */
        if (i >= cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) {
                free(digits);
                free(out);
                return next;      /* keep the computed value even if we can't grow */
            }
            cache = tmp;
            cache_cap = newcap;
        }

        cache[i] = next;
        cache_len = i + 1;

        free(digits);
        free(out);
    }

    return cache[(size_t)n];
}
