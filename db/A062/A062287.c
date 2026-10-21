/*
 * A062287  Palindromic numbers with even digits.
 * https://oeis.org/A062287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 114, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: palindromic numbers consisting only of even decimal digits
   (including 0 as the first term).  a(1)=0, a(2)=2, a(3)=4, a(4)=6, a(5)=8,
   a(6)=22, a(7)=44, ...  The function generates the terms by iterating
   through increasing digit length and constructing palindromes in
   ascending numeric order. */

#include <stddef.h>
#include <stdint.h>

static const uint8_t EVEN_DIGITS[5] = {0, 2, 4, 6, 8};

/* Compute 5^exp for 0 <= exp <= 20 using a simple loop.
   The result fits in idx_t for exp <= 20 (5^20 < 2^64). */
static idx_t pow5(idx_t exp)
{
    idx_t result = 1;
    while (exp--) result *= 5;
    return result;
}

/* Return the n-th term of the sequence, n >= 1. */
seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1) return (seq_t)0;

    /* Consider only positive palindromes and shift index accordingly. */
    idx_t pos = n - 1;                /* 1‑based index among positive terms */

    /* Length 1 (positive) contributes 4 terms: 2,4,6,8. */
    idx_t len = 1;
    idx_t count;

    /* Determine the length L containing the desired term. */
    while (1) {
        /* For length len, number of free digits (first half) */
        idx_t half = (len + 1) / 2;
        /* Count of palindromes of this length: first digit has 4 options,
           remaining (half-1) digits have 5 options each. */
        count = 4 * pow5(half - 1);

        if (pos <= count) break;     /* found the correct length */
        pos -= count;
        ++len;
    }

    /* Build the first half digits of the palindrome. */
    idx_t half = (len + 1) / 2;
    uint8_t first_half[32];   /* half will never exceed 32 for reasonable n */

    /* pos is 1‑based within this length; convert to 0‑based index. */
    idx_t index0 = pos - 1;

    /* Determine the first digit index: 0..3 corresponding to 2,4,6,8. */
    idx_t pow_half_minus1 = pow5(half - 1);
    idx_t first_digit_index = index0 / pow_half_minus1;
    first_half[0] = EVEN_DIGITS[1 + first_digit_index];   /* skip 0 */

    /* Remaining digits (if any) are encoded in base‑5. */
    idx_t rest = index0 % pow_half_minus1;
    for (idx_t pos_digit = 1; pos_digit < half; ++pos_digit) {
        idx_t exp = half - 1 - pos_digit;
        idx_t base = pow5(exp);
        idx_t digit_index = rest / base;
        rest %= base;
        first_half[pos_digit] = EVEN_DIGITS[digit_index];
    }

    /* Construct the full palindrome by mirroring the first half. */
    seq_t result = 0;
    /* Append the first half. */
    for (idx_t i = 0; i < half; ++i)
        result = result * 10 + first_half[i];

    /* Append the reversed second half. */
    for (idx_t i = len % 2 ? half - 2 : half - 1; i < half; --i)
        result = result * 10 + first_half[i];

    return result;
}
