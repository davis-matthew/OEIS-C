/*
 * A121627  Real part of a complex operation analogous to the factorials.
 * https://oeis.org/A121627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n/2) * n! * cos(nπ/4). Computed via recurrence on the
 *            real part of the complex product k(n), where k(1) = 1 + i and
 *            k(n) = (n + n i) * k(n−1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined externally by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; guard against caller error */
        return 0;

    /* real and imaginary parts of k(n) */
    seq_t x = 1;  /* real part */
    seq_t y = 1;  /* imaginary part */

    if (n == 1)
        return x;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t temp = (seq_t)i * (x - y);          /* x_i = i (x - y)      */
        y = (seq_t)i * (x + y);                    /* y_i = i (x + y)      */
        x = temp;
    }
    return x;
}
