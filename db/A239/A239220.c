/*
 * A239220  Cubes that are not divisible by any of their nonzero digits.
 * https://oeis.org/A239220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = m(n)^3 where m(n) is the nth integer whose cube has no
 *            non‑zero digit that divides the cube itself. The function
 *            searches for the required cubes directly
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_has_no_digit_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/cube_has_no_digit_divisor.h"

seq_t a(idx_t n)
{
    idx_t m = 1;            /* candidate integer whose cube we test */
    idx_t found = 0;        /* number of suitable cubes found so far */
    seq_t result = 0;       /* cube of the nth suitable integer */

    while (found < n) {
        seq_t cube = (seq_t)m * (seq_t)m * (seq_t)m;
        if (cube_has_no_digit_divisor(cube)) {
            ++found;
            result = cube;
        }
        ++m;
    }

    return result;
}
