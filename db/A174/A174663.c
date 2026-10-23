/*
 * A174663  a(n) is the number of solutions to the congruence Sum_{k=1..n} x_k
 *          == 1 (mod 2n), where x_k are distinct elements of the set {0, 1,
 *          ..., 2n}, k = 1..n.
 * https://oeis.org/A174663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)!/2)*Σ_{d|n} (-1)^(n+d) * μ(n/d) * C(2d,d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binom, factorial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/binom.h"     /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/factorial.h" /* seq_t factorial(idx_t n); */
#include "../utils/mobius.h"    /* seq_t mobius(idx_t n); */

seq_t a(idx_t n)
{
    /* For n = 0 (outside the OEIS range) return 0. */
    if (n == 0) return 0;

    seq_t sum = 0;

    /* Enumerate all positive divisors d of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* Process divisor d. */
        {
            idx_t d1 = d;
            seq_t bin = binom(2 * d1, d1);          /* C(2d,d)  */
            seq_t mu  = mobius(n / d1);            /* μ(n/d)    */
            int    exp = (int)(n + d1) & 1;         /* parity of (n+d) */
            seq_t sign = (exp ? -1 : 1);            /* (-1)^(n+d) */
            sum += bin * mu * sign;
        }

        /* Process the complementary divisor if distinct. */
        idx_t d2 = n / d;
        if (d2 != d) {
            seq_t bin = binom(2 * d2, d2);
            seq_t mu  = mobius(n / d2);
            int    exp = (int)(n + d2) & 1;
            seq_t sign = (exp ? -1 : 1);
            sum += bin * mu * sign;
        }
    }

    /* Multiply by (n-1)! / 2. */
    seq_t fact = factorial(n - 1);
    seq_t result = fact * sum;
    result /= 2;            /* guaranteed integer division */

    return result;
}
