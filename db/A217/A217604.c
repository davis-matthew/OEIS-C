/*
 * A217604  Primes or negative values of primes of the form 59*n^2 - 1873*n +
 *          8941 for n>=0.
 * https://oeis.org/A217604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = the nth value of 59*k^2 – 1873*k + 8941 that is a prime
 *            (with sign preserved; negative primes are returned negative).
 *            a(1) corresponds to k = 0. Uses the provided primality test
 *            is_prime_big(seq_t)
 * Range:     i8 none, i16 n <= 41, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset not defined for n<1 */
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t val = (seq_t)59 * (seq_t)k * (seq_t)k
                  - (seq_t)1873 * (seq_t)k
                  + (seq_t)8941;
        seq_t abs_val = val >= 0 ? val : -val;
        if (is_prime_big(abs_val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
