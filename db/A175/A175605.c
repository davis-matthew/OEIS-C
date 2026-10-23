/*
 * A175605  a(n) = concatenation of n^3 with itself.
 * https://oeis.org/A175605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n^3, n^3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function concatenates the cube of n with itself:
 *   a(n) = n^3 * 10^digit_len(n^3) + n^3
 * where digit_len(x) returns the number of decimal digits of x.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Compute n^3 in seq_t */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Determine number of decimal digits in cube */
    int d = digit_len(cube);

    /* 10^d as seq_t */
    seq_t pow10d = pow10(d);

    /* Concatenate cube with itself */
    return cube * pow10d + cube;
}
