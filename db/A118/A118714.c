/*
 * A118714  Determinant of n X n matrix whose diagonal contains the first n
 *          tetrahedral numbers and all other elements are 1's.
 * https://oeis.org/A118714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1 * a(2) = 3 * For n >= 1: a(n+2) = a(n+1) * ((n+1) *
 *            ((n+1)^2 + 8*(n+1) + 18) / 6) * Equivalent product form: * a(n)
 *            = ∏_{i=1}^{n-1} i * (i^2 + 6i + 11) / 6
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* not defined in OEIS, return 0 as placeholder */
        return 0;
    seq_t result = 1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t term = ii * (ii * ii + (seq_t)6 * ii + (seq_t)11);
        result *= term;
        result /= 6;   /* exact division, term is always divisible by 6 */
    }
    return result;
}
