/*
 * A089451  a(n) = mu(prime(n)-1), where mu is the Moebius function (A008683).
 * https://oeis.org/A089451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(prime(n)-1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mobius.h"

seq_t a(idx_t n) {
    /* get the nth prime */
    uint32_t p = get_prime(n);
    /* compute the Möbius value of p-1 */
    return mobius((idx_t)(p - 1));
}
