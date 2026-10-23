/*
 * A181364  Fibonacci numbers whose digits, when squared, sum to a prime.
 * https://oeis.org/A181364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   append_candidate, digit_len, get_digits, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate Fibonacci numbers and collect those whose sum
   of squares of decimal digits is prime.  Indexing follows the OEIS
   convention: a(1) is the first such Fibonacci number. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

/* Test whether the sum of squares of the decimal digits of x is a prime. */
static bool digit_square_sum_is_prime(seq_t x)
{
    /* number of decimal digits */
    int d = digit_len(x);
    /* Reuse a static buffer big enough for the largest number seen so far. */
    static uint8_t *buf = NULL;
    static size_t buf_cap = 0;
    if ((size_t)d > buf_cap) {
        uint8_t *nbuf = (uint8_t *)realloc(buf, (size_t)d);
        if (!nbuf) return false;
        buf = nbuf;
        buf_cap = (size_t)d;
    }
    size_t len = 0;
    get_digits(x, buf, &len);
    uint64_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t digit = buf[i];
        sum += (uint64_t)digit * (uint64_t)digit;
    }
    return is_prime_small((seq_t)sum);
}

seq_t a(idx_t n)
{
    /* static state kept between calls */
    static seq_t *qual = NULL;
    static size_t qual_len = 0;
    static size_t qual_cap = 0;
    static seq_t f_prev = 0;
    static seq_t f_curr = 1;
    static bool initialized = false;

    if (!initialized) {
        qual = NULL;
        qual_len = 0;
        qual_cap = 0;
        f_prev = 0;
        f_curr = 1;
        initialized = true;
    }

    /* Extend the cache until it contains at least n qualifying Fibonacci numbers. */
    while (qual_len < n) {
        if (digit_square_sum_is_prime(f_curr))
            append_candidate(&qual, &qual_len, &qual_cap, f_curr);
        /* Advance one Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }

    return qual[n - 1];
}
