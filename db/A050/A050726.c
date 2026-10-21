/*
 * A050726  Decimal expansion of 5^n contains no pair of consecutive equal
 *          digits (probably finite).
 * https://oeis.org/A050726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A050726.
   The sequence contains all non‑negative integers n such that
   the decimal representation of 5^n contains no pair of consecutive
   equal digits.  The function computes the n‑th term by
   generating successive powers of 5 in decimal form and
   recording those exponents for which the digit string has no
   adjacent repeats.  No pre‑computed table is used.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
/* seq_t and idx_t are supplied by the build environment */
static unsigned char *pow_digits = NULL;      /* decimal digits, LSB first */
static size_t pd_len = 0, pd_cap = 0;
static idx_t last_exp = 0;                    /* exponent of current pow_digits */
static seq_t *cache = NULL;                   /* cached a(k) values */
static size_t cache_len = 0, cache_cap = 0;
static bool initialized = false;

/* Ensure capacity of an array of unsigned char digits */
static void ensure_pd_cap(size_t needed)
{
    if (needed > pd_cap) {
        size_t new_cap = pd_cap ? pd_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        unsigned char *tmp = (unsigned char *)realloc(pow_digits, new_cap);
        if (!tmp) abort();
        pow_digits = tmp;
        pd_cap = new_cap;
    }
}

/* Ensure capacity of the cache array */
static void ensure_cache_cap(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Multiply the current decimal number (stored in pow_digits)
   by 5 and update the digit array. */
static void multiply_by_five(void)
{
    int carry = 0;
    for (size_t i = 0; i < pd_len; ++i) {
        int prod = pow_digits[i] * 5 + carry;
        pow_digits[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry > 0) {
        ensure_pd_cap(pd_len + 1);
        pow_digits[pd_len++] = carry % 10;
        carry /= 10;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise power of 5 to 5^0 = 1 */
        ensure_pd_cap(1);
        pow_digits[0] = 1;
        pd_len = 1;
        last_exp = 0;

        /* initialise cache with first term a(0) = 0 */
        ensure_cache_cap(1);
        cache[0] = (seq_t)0;
        cache_len = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n+1 cached values */
    while (cache_len <= n) {
        /* next exponent */
        ++last_exp;
        multiply_by_five();

        /* test for consecutive equal digits */
        bool ok = true;
        for (size_t i = 0; i + 1 < pd_len; ++i) {
            if (pow_digits[i] == pow_digits[i + 1]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_exp;
        }
    }

    return cache[n];
}
