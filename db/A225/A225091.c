/*
 * A225091  The odd part of the digit sum of 7^n.
 * https://oeis.org/A225091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A225091: a(n) = odd_part( digit_sum( 7^n ) )   (offset 0) */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/odd_part.h"

static uint8_t *digits = NULL;   /* base‑10 digits of 7^current_n, least significant first */
static idx_t digits_len = 0;     /* number of digits in use   */
static idx_t digits_cap = 0;     /* allocated capacity of digits */

static seq_t *cache = NULL;     /* cached a(k) for k <= current_n */
static idx_t cache_len = 0;     /* number of cached values in use   */
static idx_t cache_cap = 0;     /* allocated capacity of cache      */

static idx_t current_n = 0;     /* last exponent for which digits are computed */

/* Ensure 'digits' has capacity at least 'need' digits. */
static void ensure_digits_cap(idx_t need)
{
    if (need > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap : 1;
        while (new_cap < (size_t)need) new_cap <<= 1;
        uint8_t *newptr = (uint8_t *)realloc(digits, new_cap);
        if (!newptr) abort();
        digits = newptr;
        digits_cap = (idx_t)new_cap;
    }
}

/* Ensure 'cache' has capacity at least 'need' seq_t entries. */
static void ensure_cache_cap(idx_t need)
{
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)need) new_cap <<= 1;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) abort();
        cache = newptr;
        cache_cap = (idx_t)new_cap;
    }
}

/* Initialise static data structures on first call. */
static void init(void)
{
    if (digits == NULL) {
        /* 7^0 = 1 */
        digits_cap = 1;
        digits = (uint8_t *)malloc(digits_cap);
        if (!digits) abort();
        digits_len = 1;
        digits[0] = 1;

        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 1;
        cache[0] = 1;                 /* a(0) = 1 */

        current_n = 0;
    }
}

/* Multiply the current big integer (7^current_n) by 7. */
static void mul_by_7(void)
{
    uint32_t carry = 0;
    for (idx_t i = 0; i < digits_len; ++i) {
        uint32_t prod = (uint32_t)digits[i] * 7 + carry;
        digits[i] = prod % 10;
        carry    = prod / 10;
    }
    while (carry) {
        ensure_digits_cap(digits_len + 1);
        digits[digits_len++] = carry % 10;
        carry /= 10;
    }
}

/* Compute the digit sum of the current big integer. */
static uint64_t digit_sum(void)
{
    uint64_t sum = 0;
    for (idx_t i = 0; i < digits_len; ++i) {
        sum += digits[i];
    }
    return sum;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    init();

    if (n < cache_len) return cache[n];

    for (idx_t k = current_n + 1; k <= n; ++k) {
        mul_by_7();                                      /* 7^k now stored */
        uint64_t sum64 = digit_sum();                    /* sum as 64‑bit integer */
        seq_t odd = odd_part((seq_t)sum64);              /* compute odd part */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len++] = odd;
    }
    current_n = n;
    return cache[n];
}
