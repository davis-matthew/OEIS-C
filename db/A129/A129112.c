/*
 * A129112  Decimal expansion of constant equal to concatenated semiprimes.
 * https://oeis.org/A129112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, cons, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A129112 – decimal expansion of the concatenated semiprimes.
   a(1) is the first digit after the decimal point of the infinite word
   formed by writing all semiprimes (the product of two primes, not
   necessarily distinct) in increasing order and concatenating them. */

/* The routine builds the sequence lazily and caches the digits so that
   multiple calls in arbitrary order are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for the digits and the next integer to test */
static seq_t *digits = NULL;
static size_t digits_len = 0;
static size_t digits_cap = 0;
static uint64_t next_num = 4;   /* smallest possible semiprime */

/* ensure the digit buffer has at least 'need' capacity */
static void ensure_digits_cap(size_t need)
{
    if (need <= digits_cap) return;
    size_t new_cap = digits_cap ? (digits_cap << 1) : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
    digits = tmp;
    digits_cap = new_cap;
}

/* append the decimal digits of a positive integer to the buffer */
static void append_number_digits(uint64_t value)
{
    uint8_t stack[20];
    size_t s = 0;
    if (value == 0) stack[s++] = 0;
    while (value > 0) {
        stack[s++] = (uint8_t)(value % 10);
        value /= 10;
    }
    for (size_t i = 0; i < s; ++i) {
        ensure_digits_cap(digits_len + 1);
        digits[digits_len++] = (seq_t)(stack[s - 1 - i]); /* reversed order */
    }
}

/* test whether an unsigned 64‑bit integer is a semiprime */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;          /* 4 = 2×2 is the smallest semiprime */
    int factors = 0;
    uint64_t t = x;
    for (uint64_t p = 2; p * p <= t; ++p) {
        while (t % p == 0) {
            ++factors;
            if (factors > 2) return false;
            t /= p;
        }
    }
    if (t > 1) ++factors;
    return factors == 2;
}

/* main function required by the exercise */
seq_t a(idx_t n)
{
    /* guard against zero (though the problem states n ≥ 1) */
    if (n == 0) return 0;

    /* extend cache until we have at least 'n' digits */
    while (digits_len < (size_t)n) {
        uint64_t x = next_num;
        while (!is_semiprime(x)) ++x;
        next_num = x + 1;
        append_number_digits(x);
    }
    return digits[(size_t)(n - 1)];
}
