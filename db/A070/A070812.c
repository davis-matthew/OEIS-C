/*
 * A070812  a(n) = phi(gpf(n)) - gpf(phi(n)) = A000010(A006530(n)) -
 *          A006530(A000010(n)).
 * https://oeis.org/A070812
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(gpf(n)) - gpf(phi(n)) (for n ≥ 3)
 * Range:     i8 n <= 145, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gpf, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* values for n < 3 are outside the declared offset; return 0 safely */
    if (n < 3) return 0;

    /* gpf(n) – largest prime factor of n */
    uint64_t g1 = gpf((uint64_t)n);
    /* φ(gpf(n)) */
    seq_t phi_g1 = phi(g1);

    /* φ(n) */
    seq_t phi_n = phi(n);
    /* gpf(φ(n)) – largest prime factor of φ(n) */
    uint64_t g2 = gpf((uint64_t)phi_n);

    return phi_g1 - (seq_t)g2;
}
