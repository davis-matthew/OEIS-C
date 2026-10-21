/*
 * A097202  Numbers of the form 5^p - p^5 for p prime.
 * https://oeis.org/A097202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 5^p(n) - p(n)^5, where p(n) is the n‑th prime
 * Range:     i8 n <= 3, i16 n <= 3, i32 n <= 6, i64 n <= 9, i128 n <= 13,
 *            big any n
 * Helpers:   get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t p = get_prime(n);
    seq_t term1 = pow_int(5, p);
    seq_t term2 = pow_int(p, 5);
    return term1 - term2;
}
