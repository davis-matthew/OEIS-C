/*
 * A151763  If n is a prime == 1 mod 4 then a(n) = 1, if n is a prime == 3 mod
 *          4 then a(n) = -1, otherwise a(n) = 0.
 * https://oeis.org/A151763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n is prime and n ≡ 1 (mod 4); * -1 if n is prime and
 *            n ≡ 3 (mod 4); * 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;           /* 1 and 0 are not prime */
    if ((n % 4) == 1) {
        return is_prime(n) ? (seq_t)1 : (seq_t)0;
    }
    if ((n % 4) == 3) {
        return is_prime(n) ? (seq_t)-1 : (seq_t)0;
    }
    return (seq_t)0;                       /* n ≡ 2 (mod 4) or other cases */
}
