/*
 * A071240  Arithmetic mean of k and R(k) where k is a number using only odd
 *          digits and R(k) is its digit reversal (A004086).
 * https://oeis.org/A071240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (k + R(k))/2 where k is the n-th number using only digits
 *            1,3,5,7,9 and R(k) is the digit reversal of k
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t nth_odd_digit_number(idx_t n)
{
    /* Find minimal length len such that sum_{i=1}^{len} 5^i > n. */
    idx_t len = 1;
    idx_t total = 0;
    idx_t step = 5;          /* 5^len */
    while (total + step <= n) {
        total += step;
        ++len;
        step *= 5;
    }

    /* offset within numbers of this length */
    idx_t offset = n - total;          /* 0 <= offset < 5^len */

    /* Convert offset to base-5 representation with len digits
       (including leading zeros). Each base-5 digit d (0..4)
       maps to decimal digit 1 + 2*d. */
    seq_t k = 0;
    seq_t mult = 1;  /* 10^(len-1) */
    /* precompute 10^(len-1) in seq_t */
    for (idx_t i = 1; i < len; ++i) {
        mult *= (seq_t)10;
    }

    for (idx_t i = 0; i < len; ++i, mult /= (seq_t)10) {
        /* Extract most significant digit first */
        idx_t divisor = 1;
        for (idx_t j = len; j > i+1; --j) divisor *= 5;

        idx_t d = offset / divisor;
        offset %= divisor;
        seq_t digit = 1 + (seq_t)(2 * d);
        k += digit * mult;
    }
    return k;
}

static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * (seq_t)10 + x % (seq_t)10;
        x /= (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t k = nth_odd_digit_number(n);
    seq_t rev = reverse_digits(k);
    return (k + rev) / (seq_t)2;
}
