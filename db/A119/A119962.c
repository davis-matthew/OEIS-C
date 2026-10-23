/*
 * A119962  Duplicate of A014234.
 * https://oeis.org/A119962
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = largest prime less than 2^n (with offset 2)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   is_prime_big, pow2, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/seq_from_str.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Undefined for n < 2; return 0 as a harmless placeholder */
    if (n < 2) return 0;

    /* 2^n */
    seq_t power = pow2(n);

    /* 2^n - 1 (first candidate, always odd for n >= 1) */
    seq_t one = seq_from_str("1");
    seq_t candidate = power - one;

    /* Step by 2 because all primes > 2 are odd */
    seq_t two = seq_from_str("2");
    while (!is_prime_big(candidate)) {
        candidate = candidate - two;
    }
    return candidate;
}
