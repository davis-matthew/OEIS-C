/*
 * A214405  Numbers of the form (4k+3)^2-8 or (4k+5)^2+4.
 * https://oeis.org/A214405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1)*((n-1)+3) - 6*(-1)^(n-1) + 7 (equivalently, a(n)
 *            = 4*(n-1)*(n+2) - 6*(-1)^(n-1) + 7)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 (outside the defined offset) give a benign value */
    if (n == 0)
        return 0;

    /* index used in the formula: m = n-1 */
    seq_t m = (seq_t)(n - 1);
    /* term = 4*m*(m+3) + 7 */
    seq_t term = 4 * m * (m + 3) + 7;

    /* Add 6 if m is odd (since -6*(-1)^(odd) = +6)
       Subtract 6 if m is even (since -6*(-1)^(even) = -6) */
    uint64_t parity = (uint64_t)(n - 1) & 1ULL;
    if (parity) {
        term += 6;
    } else {
        term -= 6;
    }

    return term;
}
