/*
 * A240453  Greatest prime divisors of the palindromes with an even number of
 *          digits.
 * https://oeis.org/A240453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = greatest prime divisor of the palindrome formed by
 *            concatenating n and rev(n)
 * Range:     u8 n <= 9, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9644 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    /* Convert index to native 64‑bit for intermediate calculations. */
    uint64_t nn = (uint64_t)n;

    /* Reverse of the decimal representation of n. */
    uint64_t rev_n = reverse_digits_idx(nn);

    /* Count decimal digits of n (needed for the power of ten). */
    uint64_t tmp = nn;
    int digits = 0;
    do {
        digits++;
        tmp /= 10;
    } while (tmp > 0);

    /* Compute 10^digits as a seq_t. */
    seq_t pow = pow10((idx_t)digits);

    /* Build the palindrome: n followed by rev(n). */
    seq_t palindrome = (seq_t)nn * pow + (seq_t)rev_n;

    /* The palindrome fits in 64 bits for the required input range. */
    uint64_t pal64 = (uint64_t)palindrome;

    /* Greatest prime divisor of the 64‑bit value. */
    uint64_t gpf = largest_prime_divisor(pal64);

    return (seq_t)gpf;
}
