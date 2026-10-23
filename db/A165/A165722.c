/*
 * A165722  Positive integers k such that the sum of decimal digits of (16^k -
 *          1) equals 6*k.
 * https://oeis.org/A165722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   ensure_terms_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:
 *  Sequence A165722:  a(n) = k-th positive integer k with
 *  sum_of_decimal_digits(16^k - 1) == 6*k.
 *  The function generates the sequence on demand, storing
 *  previously found terms in a static array.  It uses a
 *  decimal digit representation of successive powers
 *  16^k = 2^(4k) and computes the digit sum of 16^k-1
 *  by subtracting 1 on the fly.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_terms_capacity.h"

/* Helper for doubling dynamically sized arrays */
static void ensure_digits_capacity(size_t need)
{
    static size_t cap = 0;
    static uint8_t *digits = NULL;
    if (digits == NULL) {
        cap = 64;
        digits = (uint8_t *)malloc(cap);
    }
    if (need > cap) {
        size_t newcap = cap;
        while (newcap < need) newcap *= 2;
        digits = (uint8_t *)realloc(digits, newcap);
        cap = newcap;
    }
    /* expose to outer static variables */
    *(&digits) = digits;
    *(&cap) = cap;
}

static seq_t *terms = NULL;          /* stored k values satisfying the condition */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of the terms array */

static uint8_t *digits = NULL;       /* decimal digits of 16^current_k (LSB first) */
static size_t digits_len = 0;        /* number of used digits */
static size_t digits_cap = 0;        /* capacity of the digits array */

static uint64_t current_k = 0;       /* current exponent k (for 16^k) */

static void init_static_data(void)
{
    if (digits == NULL) {
        digits_cap = 64;
        digits = (uint8_t *)malloc(digits_cap);
        digits[0] = 1;          /* 16^0 = 1 */
        digits_len = 1;
    }
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }
}

/* Multiply current digits by 16 (i.e., compute 16^(current_k+1)) */
static void multiply_digits_by_16(void)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint32_t tmp = digits[i] * 16 + carry;
        digits[i] = tmp % 10;
        carry = tmp / 10;
    }
    while (carry > 0) {
        if (digits_len == digits_cap) {
            digits_cap *= 2;
            digits = (uint8_t *)realloc(digits, digits_cap);
        }
        digits[digits_len++] = carry % 10;
        carry /= 10;
    }
}

/* Compute sum of decimal digits of 16^current_k - 1 without modifying digits */
static seq_t sum_digits_of_minus_one(void)
{
    seq_t sum = 0;
    uint8_t borrow = 1; /* subtract 1 */
    for (size_t i = 0; i < digits_len; ++i) {
        uint8_t d = digits[i];
        uint8_t newd;
        if (borrow) {
            if (d == 0) {
                newd = 9;
                borrow = 1;
            } else {
                newd = d - 1;
                borrow = 0;
            }
        } else {
            newd = d;
        }
        sum += newd;
    }
    /* For 16^k (k>=0), borrow will always resolve, so no extra handling needed. */
    return sum;
}

/* Append a new term to the static terms array */
static void append_term(seq_t k)
{
    if (terms_len == terms_cap) {
        terms_cap *= 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = k;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_static_data();

    /* Generate terms until we have at least n */
    while (terms_len < (size_t)n) {
        /* Advance to next k */
        multiply_digits_by_16();
        ++current_k;

        /* Compute digit sum of 16^k - 1 */
        seq_t sum = sum_digits_of_minus_one();

        /* Check condition: sum == 6*k */
        if (sum == (seq_t)current_k * 6) {
            append_term((seq_t)current_k);
        }

        /* Safety cutoff: if k grows too large and no new terms are found,
           stop to avoid infinite loop (method assumes the sequence ends). */
        if (current_k > 200000 && terms_len < n) {
            /* No further terms expected; break out of loop */
            break;
        }
    }

    if (n > terms_len) return 0; /* term not defined beyond known range */
    return terms[n-1];
}
