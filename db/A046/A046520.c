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
 * Helpers:   divisor_count_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t ph    = phi(n);
    seq_t tau   = (seq_t)divisor_count_u64(n);
    return sigma - ph - tau;
}
