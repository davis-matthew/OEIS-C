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
 * Validated: standard, pass, 47 terms through n = 9808 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Test whether a cube has any non‑zero digit divisor. */
static bool cube_has_no_digit_divisor(seq_t cube)
{
    seq_t temp = cube;
    while (temp > 0) {
        unsigned dig = (unsigned)(temp % (seq_t)10);
        temp /= (seq_t)10;
        if (dig != 0 && cube % (seq_t)dig == 0) {
            return false;
        }
    }
    return true;
}

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
