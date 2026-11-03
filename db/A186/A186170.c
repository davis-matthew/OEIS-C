/*
 * A186170  Number of prime factors times n minus sum of divisors.
 * https://oeis.org/A186170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Ω(n)*n - σ(n) where Ω(n) is the total number of prime
 *            factors of n counted with multiplicity, and σ(n) is the sum of
 *            all divisors of n
 * Range:     i8 n <= 63, i16 n <= 4095, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 9459 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t omega = bigomega(n);   /* total number of prime factors (with multiplicity) */
    seq_t sigma = sigma_of(n);   /* sum of divisors */
    seq_t result = (seq_t)n * omega - sigma;
    return result;
}
