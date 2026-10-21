/*
 * A073497  a(n) = n^2 - prime(n).
 * https://oeis.org/A073497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - prime(n)
 * Range:     i8 n <= 12, i16 n <= 184, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* nth prime (prime(1)=2) */
    uint32_t p = get_prime(n);
    /* compute n^2 in the target integer type */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    return n_sq - (seq_t)p;
}
