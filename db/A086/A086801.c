/*
 * A086801  a(n) = prime(n) - 3.
 * https://oeis.org/A086801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = prime(n) - 3
 * Range:     i8 n <= 31, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            // offset is 1; behaviour for n<1 is irrelevant
    return (seq_t)get_prime(n) - (seq_t)3;
}
