/*
 * A046520  a(n) = (sum of divisors of n) - phi(n) - (number of divisors of
 *          n).
 * https://oeis.org/A046520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(n) - phi(n) - tau(n)
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9761 (2026-11-01)
 * Helpers:   num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/totient.h"

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

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t ph    = totient(n);
    seq_t tau   = (seq_t)num_divisors(n);
    return sigma - ph - tau;
}
