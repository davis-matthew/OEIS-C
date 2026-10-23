/*
 * A135675  a(n) = ceiling(n^(4/3) - n).
 * https://oeis.org/A135675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(4/3) - n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    /* cast index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* compute n^4; uses seq_t arithmetic which may be big-integer-enabled */
    seq_t n2  = n_seq * n_seq;
    seq_t n4  = n2 * n2;

    /* floor of n^(4/3) is floor((n^4)^(1/3)) */
    seq_t c   = floor_cube_root(n4);

    /* difference to subtract integer n */
    seq_t diff = c - n_seq;

    /* check if n is a perfect cube */
    seq_t t   = floor_cube_root(n_seq);
    seq_t t3  = t * t * t;
    if (t3 == n_seq)
        return diff;          /* n^(4/3) is integer, so ceil equals floor */
    else
        return diff + 1;      /* otherwise add one to get the ceiling */
}
