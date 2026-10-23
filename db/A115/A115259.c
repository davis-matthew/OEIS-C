/*
 * A115259  Difference between the sum of digits in odd positions and the sum
 *          of digits in even positions of prime numbers.
 * https://oeis.org/A115259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = difference between sum of digits in odd positions * and
 *            sum of digits in even positions (least significant digit * is
 *            position 1) of the nth prime
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    uint64_t p = (uint64_t)get_prime(n);
    seq_t odd = 0;
    seq_t even = 0;
    int pos = 1;  /* position counter, starting at 1 for least significant digit */
    while (p > 0) {
        seq_t digit = (seq_t)(p % 10);
        if (pos & 1)
            odd += digit;
        else
            even += digit;
        p /= 10;
        ++pos;
    }
    return odd - even;
}
