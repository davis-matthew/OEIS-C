/*
 * A131616  Numbers k such that the decimal expansion of 3^k contains no 6.
 * https://oeis.org/A131616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the n‑th exponent k such that the decimal expansion of
   3^k contains no digit 6.  The sequence is produced by scanning
   increasing values of k, building 3^k digit by digit and recording the
   indices for which the digit 6 does not appear. */
/* Note: The implementation uses only the arithmetic types seq_t and
   idx_t supplied by the build.  It relies on the standard library
   functions malloc() and realloc(), and on basic integer operations. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the stream of discovered exponents and the
   current representation of 3^k in decimal. */
static idx_t *cache      = NULL;      /* cached exponents (k values) */
static size_t cache_size = 0;          /* number of cached terms      */
static size_t cache_cap  = 0;          /* capacity of the cache array */

static uint8_t *digits = NULL;         /* digits of the current power of 3 (LSB first) */
static size_t digits_len = 0;          /* number of used digits   */
static size_t digits_cap = 0;          /* capacity of the digits array */

/* The current exponent being represented in digits. */
static idx_t current_k = 0;

/* Flag to indicate lazy initialization. */
static bool initialized = false;

/* ------------------------------------------------------------------ */
/* Ensure the cache array can hold at least 'needed' entries.         */
/* ------------------------------------------------------------------ */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Ensure the digits array can hold at least 'needed' digits.         */
/* ------------------------------------------------------------------ */
static void ensure_digits_capacity(size_t needed)
{
    if (needed <= digits_cap)
        return;
    size_t new_cap = digits_cap ? digits_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    digits = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
    digits_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Multiply the current decimal representation by 3.  The array is
   kept in least‑significant‑digit first order. */
static void multiply_by_3(void)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i)
    {
        uint16_t prod = (uint16_t)digits[i] * 3 + carry;
        digits[i] = (uint8_t)(prod % 10);
        carry    = (uint16_t)(prod / 10);
    }
    while (carry > 0)
    {
        ensure_digits_capacity(digits_len + 1);
        digits[digits_len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* ------------------------------------------------------------------ */
/* Check whether the current number (digits) contains a decimal digit
   6. */
static bool has_digit_six(void)
{
    for (size_t i = 0; i < digits_len; ++i)
        if (digits[i] == 6)
            return true;
    return false;
}

/* ------------------------------------------------------------------ */
/* Return the n-th term of the sequence.  n is 1‑based.                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* out of range – return 0 for safety */
        return (seq_t)0;

    /* Lazy initialization on first call. */
    if (!initialized)
    {
        digits_cap = 64;
        digits = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));
        digits_len = 1;
        digits[0] = 1;            /* 3^0 = 1 */

        cache_cap = 64;
        cache = (idx_t *)malloc(cache_cap * sizeof(idx_t));
        cache_size = 0;

        current_k = 0;
        initialized = true;
    }

    /* Generate terms until we have at least n cached values. */
    while (cache_size < n)
    {
        if (!has_digit_six())
        {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = current_k;
        }
        multiply_by_3();
        ++current_k;
    }

    return (seq_t)cache[n - 1];
}
