/*
 * A226930  Second differences give the sequence itself, but read
 *          digit-by-digit.
 * https://oeis.org/A226930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + d_{n-2} (for n ≥ 3) where d_i is the
 *            i‑th digit of the concatenated decimal representation of the
 *            sequence. Initial values: a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 13, u16 n <= 172, u32 n <= 9673, u64 n <= 9673,
 *            u128 n <= 9673, big any n
 * Helpers:   digit_len, seq_alloc_new_array, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

/* Cached values of a(n) for 1 ≤ n ≤ cache_len. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Concatenated decimal digits of the sequence, stored in 0‑based order. */
static uint8_t *digits = NULL;
static size_t digits_len = 0;
static size_t digits_cap = 0;

/* Temporary buffer for converting a term to a string. */
static char *tmp_str = NULL;
static size_t tmp_cap = 0;

/* Ensure that cache contains terms up to n. */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_len) return;            /* already computed */

    /* ---------- Initialization for the first four terms ---------- */
    if (cache_len == 0) {
        /* Allocate space for first four terms */
        cache_cap = 4;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache_len = 4;

        /* Append digits of these terms */
        for (idx_t i = 1; i <= 4; ++i) {
            seq_t v = cache[i - 1];
            size_t dlen = (size_t)digit_len(v);
            if (dlen > tmp_cap) {
                tmp_cap = dlen * 2;
                tmp_str = (char *)realloc(tmp_str, tmp_cap);
            }
            size_t written = 0;
            seq_to_str(v, tmp_str, &written);
            for (size_t j = 0; j < written; ++j) {
                uint8_t dig = (uint8_t)(tmp_str[j] - '0');
                if (digits_len + 1 > digits_cap) {
                    size_t new_cap = digits_cap ? digits_cap * 2 : 64;
                    while (digits_len + 1 > new_cap) new_cap <<= 1;
                    digits = (uint8_t *)realloc(digits, new_cap);
                    digits_cap = new_cap;
                }
                digits[digits_len++] = dig;
            }
        }
    }

    /* ---------- Compute remaining terms up to n ---------- */
    if (cache_len < n) {
        /* Grow the cache array if necessary */
        if (cache_cap < n) {
            size_t new_cap = cache_cap ? cache_cap : 4;
            while (new_cap < n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
            seq_t val;
            if (idx <= 4) {
                val = cache[idx - 1];   /* should not happen */
            } else {
                /* d_{n-2} = digits[n-3] (0‑based) */
                seq_t digit_needed = (seq_t)digits[idx - 3];
                val = (seq_t)2 * cache[idx - 2] - cache[idx - 3] + digit_needed;
            }
            cache[idx - 1] = val;

            /* Append digits of the newly computed term */
            size_t dlen = (size_t)digit_len(val);
            if (dlen > tmp_cap) {
                tmp_cap = dlen * 2;
                tmp_str = (char *)realloc(tmp_str, tmp_cap);
            }
            size_t written = 0;
            seq_to_str(val, tmp_str, &written);
            for (size_t j = 0; j < written; ++j) {
                uint8_t dig = (uint8_t)(tmp_str[j] - '0');
                if (digits_len + 1 > digits_cap) {
                    size_t new_cap = digits_cap ? digits_cap * 2 : 64;
                    while (digits_len + 1 > new_cap) new_cap <<= 1;
                    digits = (uint8_t *)realloc(digits, new_cap);
                    digits_cap = new_cap;
                }
                digits[digits_len++] = dig;
            }
        }
        cache_len = n;
    }
}

/* Main function to be called by the judging framework. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
