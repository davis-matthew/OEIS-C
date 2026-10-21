/*
 * A075656  Incorrect duplicate of A073353.
 * https://oeis.org/A075656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n + rad(n) for n > 1; a(1) = 1. rad(n) is the product of
 *            the distinct prime factors of n
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   rad_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/rad_of.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 1; a(0) is undefined but we return 1 for safety. */
        return (seq_t)1;
    }

    seq_t rad = rad_of(n);
    return (seq_t)n + rad;
}
