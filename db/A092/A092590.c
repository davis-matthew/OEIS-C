/*
 * A092590  a(n) = A065395(A000040(n)); values of commutator of sigma and phi
 *          function at prime number arguments.
 * https://oeis.org/A092590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = sigma(prime(n)-1) - phi(prime(n)+1)
 * Range:     i8 n <= 17, i16 n <= 1404, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   calc_phi, get_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* n is 1-indexed */
    seq_t p = (seq_t)get_prime(n);          /* prime(n) */
    seq_t sigma_val = sigma_of((idx_t)(p - 1)); /* sigma(prime(n)-1) */
    seq_t phi_val = calc_phi(p + 1);        /* phi(prime(n)+1) */
    return sigma_val - phi_val;             /* difference */
}
