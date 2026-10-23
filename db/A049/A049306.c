/*
 * A049306  Numbers k such that k is a substring of 8^k.
 * https://oeis.org/A049306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer k such that the decimal representation
 *            of k * occurs as a substring of the decimal representation of
 *            8^k
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Static state shared between calls. */
static idx_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

/* Digits of 8^k (least‑significant first). */
static uint8_t *pow_digits = NULL;
static size_t pow_len = 0;
static size_t pow_cap_digits = 0;

/* Decimal string of 8^k (most‑significant first). */
static char *pow_str = NULL;
static size_t pow_cap_str = 0;

/* Increase capacity of the digit array if necessary. */
static void ensure_pow_digits_cap(size_t needed)
{
    if (pow_cap_digits < needed) {
        size_t new_cap = pow_cap_digits ? pow_cap_digits << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(pow_digits, new_cap);
        pow_digits = tmp;
        pow_cap_digits = new_cap;
    }
}

/* Increase capacity of the string buffer if necessary. */
static void ensure_pow_str_cap(size_t needed)
{
    if (pow_cap_str < needed) {
        size_t new_cap = pow_cap_str ? pow_cap_str << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        char *tmp = (char *)realloc(pow_str, new_cap);
        pow_str = tmp;
        pow_cap_str = new_cap;
    }
}

/* Increase capacity of the cache array if necessary. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Convert an idx_t integer to a decimal string in buf.
   buf must be large enough (at least 21 bytes for 64‑bit values). */
static void idx_to_str(idx_t value, char *buf, size_t *out_len)
{
    size_t pos = 0;
    do {
        buf[pos++] = (char)('0' + (value % 10));
        value /= 10;
    } while (value);
    for (size_t i = 0; i < pos / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    buf[pos] = '\0';
    if (out_len) *out_len = pos;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, undefined for 0 */

    /* Initialise static structures on first call. */
    if (cache == NULL) {
        /* 8^0 = 1. */
        pow_len = 1;
        ensure_pow_digits_cap(1);
        pow_digits[0] = 1;
        ensure_pow_str_cap(2);
        pow_str[0] = '1';
        pow_str[1] = '\0';
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        ++last_k;   /* Next k to test. */

        /* Multiply current 8^(k-1) by 8 to obtain 8^k. */
        uint16_t carry = 0;
        for (size_t i = 0; i < pow_len; ++i) {
            uint16_t prod = pow_digits[i] * 8 + carry;
            pow_digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            ensure_pow_digits_cap(pow_len + 1);
            pow_digits[pow_len++] = carry % 10;
            carry /= 10;
        }

        /* Build the decimal string for 8^k. */
        ensure_pow_str_cap(pow_len + 1);
        for (size_t i = 0; i < pow_len; ++i)
            pow_str[pow_len - 1 - i] = '0' + pow_digits[i];
        pow_str[pow_len] = '\0';

        /* Convert k to decimal string. */
        char kbuf[32];   /* 20 digits for 64‑bit plus safety. */
        size_t klen;
        idx_to_str(last_k, kbuf, &klen);

        /* Check if k is a substring of 8^k. */
        if (strstr(pow_str, kbuf) != NULL) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = last_k;
        }
    }

    /* Return the nth element (1‑based indexing). */
    return (seq_t)cache[n - 1];
}
