/*
 * A008572  Digits of powers of 12.
 * https://oeis.org/A008572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5500, u16 n <= 5500, u32 n <= 5500, u64 n <= 5500,
 *            u128 n <= 5500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008572: digits of powers of 12.
   a(n) = the n‑th digit (0‑indexed) in the concatenation of
   decimal representations of 12^k for k = 0,1,2,… */

/* The algorithm keeps the decimal digits of the current power
   of 12 in an array (least significant digit first).  For
   each new power it multiplies this array by 12 and then
   appends the digits of the new power to a growing cache
   in most significant order.  This works for both small
   and arbitrary‑precision builds. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/* cached digits of the concatenation (most significant first) */
static seq_t *digit_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* decimal digits of the current power 12^k (least significant first) */
static uint8_t *power_digits = NULL;
static size_t power_len = 0, power_cap = 0;

/* enlarge the digit cache to hold at least 'need' more digits */
static void ensure_cache_capacity(size_t need)
{
    if (cache_len + need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < cache_len + need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(digit_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        digit_cache = tmp;
        cache_cap   = new_cap;
    }
}

/* enlarge the power digit buffer to hold at least 'need' more digits */
static void ensure_power_capacity(size_t need)
{
    if (power_len + need > power_cap) {
        size_t new_cap = power_cap ? power_cap * 2 : 128;
        while (new_cap < power_len + need)
            new_cap *= 2;
        uint8_t *tmp = (uint8_t *)realloc(power_digits, new_cap * sizeof(uint8_t));
        if (!tmp) abort();
        power_digits = tmp;
        power_cap    = new_cap;
    }
}

/* append the current power (digits stored LSD→MSD) to the cache
   in most significant order. */
static void append_current_power_to_cache(void)
{
    /* digits of power_digits[power_len-1] ... power_digits[0] */
    for (ptrdiff_t i = (ptrdiff_t)power_len - 1; i >= 0; --i) {
        ensure_cache_capacity(1);
        digit_cache[cache_len++] = (seq_t)power_digits[i];
    }
}

/* multiply current power by 12 in place, updating power_digits. */
static void multiply_power_by_12(void)
{
    int carry = 0;
    for (size_t i = 0; i < power_len; ++i) {
        int t = (int)power_digits[i] * 12 + carry;
        power_digits[i] = (uint8_t)(t % 10);
        carry = t / 10;
    }
    while (carry > 0) {
        ensure_power_capacity(1);
        power_digits[power_len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

seq_t a(idx_t n)
{
    /* first call: initialise power = 12^0 = 1 */
    if (cache_len == 0) {
        ensure_cache_capacity(1);
        digit_cache[cache_len++] = (seq_t)1;

        ensure_power_capacity(1);
        power_digits[0] = 1;
        power_len = 1;
    }

    /* extend the cache until it contains at least n+1 digits */
    while (cache_len <= (size_t)n) {
        multiply_power_by_12();        /* 12^k → 12^{k+1} */
        append_current_power_to_cache();
    }

    return digit_cache[(size_t)n];
}
