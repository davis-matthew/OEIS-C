/*
 * A226289  Expansion of f(-x) * phi(x^3) in powers of x where f(), phi() are
 *          Ramanujan theta functions.
 * https://oeis.org/A226289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k∈ℤ} b(n – 3k²) with b(m) = 1 if m = 0 (-1)^j if m =
 *            j(3j±1)/2 for some j>0 0 otherwise where j runs over positive
 *            integers
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pentagonal_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pentagonal_coeff.h"

seq_t a(idx_t n)
{
    seq_t sum = pentagonal_coeff(n);          /* k = 0 term */
    for (idx_t k = 1; ; ++k) {
        idx_t sub = 3 * k * k;
        if (sub > n) break;
        sum += 2 * pentagonal_coeff(n - sub); /* ±k contribute twice */
    }
    return sum;
}
