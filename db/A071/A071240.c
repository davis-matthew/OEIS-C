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
 * Helpers:   nth_odd_digit_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_odd_digit_number.h"

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
