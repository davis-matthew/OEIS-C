/*
 * A236673  Exponents of powers of 3 that contain all ten decimal digits.
 * https://oeis.org/A236673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + 63 for n >= 44; earlier terms are found by
 *            brute‑force search
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_SPECIAL_TERMS 43          /* number of terms before the linear pattern */

static seq_t specials[MAX_SPECIAL_TERMS + 1]; /* 1‑based index: specials[0] = a(1) */
static size_t specials_len = 0;                /* number of terms already found */

/* Decimal representation of 3^k, least significant digit first */
static uint8_t digits[200];
static size_t digits_len = 0;
static idx_t current_exp = 0;                 /* exponent of the current number */

/* Global initialization flag */
static bool initialized = false;

static void init_global(void)
{
    digits_len = 1;
    digits[0] = 1;            /* 3^0 */
    current_exp = 0;
    specials_len = 0;
    initialized = true;
}

/* Check whether the current decimal number contains all digits 0..9. */
static bool contains_all_digits(void)
{
    unsigned mask = 0;
    for (size_t i = 0; i < digits_len; ++i)
        mask |= 1u << digits[i];
    return mask == 0x3FFu;    /* all ten bits set */
}

/* Multiply the current decimal number by 3. */
static void multiply_by_three(void)
{
    uint16_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint16_t prod = digits[i] * 3u + carry;
        digits[i] = prod % 10u;
        carry = prod / 10u;
    }
    while (carry) {
        digits[digits_len++] = carry % 10u;
        carry /= 10u;
    }
    ++current_exp;
    if (specials_len < MAX_SPECIAL_TERMS && contains_all_digits())
        specials[specials_len++] = current_exp;
}

/* Generate terms until at least n special terms are known. */
static void generate_terms(idx_t n)
{
    if (!initialized)
        init_global();
    while (specials_len < n && specials_len < MAX_SPECIAL_TERMS)
        multiply_by_three();
}

/* Main entry: returns the n‑th term of the OEIS sequence A236673. */
seq_t a(idx_t n)
{
    if (n <= specials_len)
        return specials[n - 1];

    if (n > MAX_SPECIAL_TERMS)
        return (seq_t)n + 63;

    /* Need more special terms. */
    generate_terms(n);
    if (n <= specials_len)
        return specials[n - 1];

    /* Fallback: linear pattern after the found terms. */
    return (seq_t)n + 63;
}
