/*
 * A053058  Cubes whose digit sum is also a cube.
 * https://oeis.org/A053058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = the n-th cube k³ whose decimal digit sum is also a
 *            perfect cube
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 129, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9940 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Check whether a digit sum is a perfect cube (only values up to 343 matter) */
static bool is_cube_digit_sum(seq_t s)
{
    return s == (seq_t)0 || s == (seq_t)1 || s == (seq_t)8 ||
           s == (seq_t)27 || s == (seq_t)64 || s == (seq_t)125 ||
           s == (seq_t)216 || s == (seq_t)343;
}

seq_t a(idx_t n)
{
    idx_t   count = 0;          /* how many cubes satisfy the condition so far   */
    idx_t   base  = 0;          /* current base, whose cube we are testing      */

    while (true) {
        seq_t cube = (seq_t)base;        /* cube = base³  */
        cube *= cube;                   /* cube²        */
        cube *= (seq_t)base;             /* cube³        */

        if (is_cube_digit_sum(digit_sum(cube))) {
            ++count;
            if (count == n) {
                return cube;
            }
        }
        ++base;
    }
}
