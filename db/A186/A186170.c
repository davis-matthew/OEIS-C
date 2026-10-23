/*
 * A186170  Number of prime factors times n minus sum of divisors.
 * https://oeis.org/A186170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Ω(n)*n - σ(n) where Ω(n) is the total number of prime
 *            factors of n counted with multiplicity, and σ(n) is the sum of
 *            all divisors of n
 * Range:     i8 n <= 63, i16 n <= 4095, i32 n <= 9459, i64 n <= 9459,
 *            i128 n <= 9459, big any n
 * Helpers:   bigomega, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    seq_t omega = bigomega(n);   /* total number of prime factors (with multiplicity) */
    seq_t sigma = sigma_of(n);   /* sum of divisors */
    seq_t result = (seq_t)n * omega - sigma;
    return result;
}
