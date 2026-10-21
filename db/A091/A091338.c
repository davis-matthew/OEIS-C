/*
 * A091338  a(n) = (3/n), where (k/n) is the Kronecker symbol.
 * https://oeis.org/A091338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (3/n) Kronecker symbol Computation: 0 if 3|n; otherwise
 *            Let n = 2^e * m (m odd, m%3 ≠ 0). Compute Jacobi(3/m) which
 *            equals +1 if m % 12 == 1 or 11 -1 if m % 12 == 5 or 7 Then (3/n)
 *            = (-1)^e * Jacobi(3/m). The sign flips once for each factor 2 in
 *            n
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 if 3 divides n */
    if (n % 3 == 0) {
        return (seq_t)0;
    }

    /* Separate powers of 2 */
    idx_t m = n;
    unsigned e = 0;
    while ((m & 1) == 0) {
        e++;
        m >>= 1;
    }

    /* Jacobi symbol for odd part */
    seq_t jacobi = 1;
    unsigned mod12 = m % 12;
    if (mod12 == 1 || mod12 == 11) {
        jacobi = 1;
    } else if (mod12 == 5 || mod12 == 7) {
        jacobi = -1;
    } else {
        /* This should never happen since m%3!=0, but guard anyway */
        jacobi = 0;
    }

    /* Sign flips for each factor of 2 */
    if (e & 1) {          /* e odd -> one flip */
        jacobi = -jacobi;
    }

    return jacobi;
}
