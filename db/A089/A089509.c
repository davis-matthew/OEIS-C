/*
 * A089509  a(n) = Kronecker(7/n).
 * https://oeis.org/A089509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Kronecker(7/n) (Kronecker symbol with numerator 7)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   jacobi_prime7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jacobi_prime7.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* Not defined for offset, return 0 */

    /* If 7 divides n the Kronecker symbol is 0. */
    if (n % 7 == 0)
        return (seq_t)0;

    /* Strip powers of 2: (7|2)=1, so they don't affect the value. */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;                  /* m is now odd */

    int j = jacobi_prime7(7, (int64_t)m);  /* m fits in int64_t */
    return (seq_t)j;          /* j is ±1 */
}
