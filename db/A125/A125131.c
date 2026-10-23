/*
 * A125131  Product 1-p, where p ranges over the prime factors of n with
 *          multiplicity.
 * https://oeis.org/A125131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = ∏_{p^e || n} (1 – p)^e (product over prime factors of n
 *            with multiplicity)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset: a(1) = 1 */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    idx_t temp = n;
    seq_t res = (seq_t)1;

    /* Trial division up to sqrt(temp) */
    for (idx_t i = (idx_t)2; i <= temp / i; ++i) {
        while (temp % i == (idx_t)0) {
            res *= ((seq_t)1 - (seq_t)i);
            temp /= i;
        }
    }

    /* If a prime factor remains */
    if (temp > (idx_t)1) {
        res *= ((seq_t)1 - (seq_t)temp);
    }

    return res;
}
