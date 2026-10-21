/*
 * A085097  Ramanujan sum c_n(3).
 * https://oeis.org/A085097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = phi(n)*mu(n/gcd(n, 3)) / phi(n/gcd(n, 3))
 * Range:     i8 n <= 8048, i16 n <= 8048, i32 n <= 8048, i64 n <= 8048,
 *            i128 n <= 8048, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* The formula simplifies to:
       • If 3 ∤ n  : a(n) = μ(n)
       • If 3 | n and 9 ∤ n : a(n) = 2·μ(n/3)
       • If 9 | n : a(n) = 3·μ(n/3)   */
    if (n % 3 != 0) {
        return mobius(n);
    }

    const idx_t m = n / 3;
    const seq_t mu_m = mobius(m);

    /* Check whether n contains 3² as a factor */
    return (n % 9 == 0) ? mu_m * 3 : mu_m * 2;
}
