/*
 * A227625  Indicator sequence of primes p > 3: k = p mod 6, if k = 5 then
 *          a(n) = -1, if k = 1 then a(n) = 1 else a(n) = 0, a(2) = -1, a(3) =
 *          1.
 * https://oeis.org/A227625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1900, i16 n <= 1900, i32 n <= 1900, i64 n <= 1900,
 *            i128 n <= 1900, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227625: a(n) = 1 if n is a prime > 3 with n % 6 == 1,
   a(n) = -1 if n is a prime > 3 with n % 6 == 5,
   a(2) = -1, a(3) = 1; otherwise a(n) = 0. */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* a(1) = 0 */
    if (n == 2) return -1;        /* a(2) = -1 */
    if (n == 3) return 1;         /* a(3) = 1 */

    if (is_prime(n)) {
        /* primes > 3 are of form 6k ± 1 */
        return (n % 6 == 5) ? -1 : 1;
    }
    return 0;
}
