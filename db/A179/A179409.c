/*
 * A179409  The number of alive cells in Conway's Game of Life on the 8x8
 *          toroidal grid, in a cyclic sequence of 48 patterns containing
 *          among other patterns, a "stairstep hexomino" and its mirror image,
 *          illustrated below.
 * https://oeis.org/A179409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179409: number of alive cells in the 8×8 toroidal Game of Life.
   The sequence is periodic with period 24; the following table contains
   the first 24 values (a(0) … a(23)). The function returns a(n) = a(n mod 24). */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic values for n = 0 .. 23 */
    static const seq_t period[24] = {
        (seq_t)6,  (seq_t)6,  (seq_t)6,  (seq_t)8,
        (seq_t)10, (seq_t)12, (seq_t)16, (seq_t)18,
        (seq_t)20, (seq_t)26, (seq_t)26, (seq_t)28,
        (seq_t)30, (seq_t)22, (seq_t)26, (seq_t)16,
        (seq_t)20, (seq_t)18, (seq_t)18, (seq_t)20,
        (seq_t)16, (seq_t)20, (seq_t)10, (seq_t)8
    };
    /* Compute index modulo 24 and return the precomputed value. */
    size_t idx = (size_t)(n % (idx_t)24);
    return period[idx];
}
