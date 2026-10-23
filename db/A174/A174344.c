/*
 * A174344  List of x-coordinates of point moving in clockwise square spiral.
 * https://oeis.org/A174344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(1) = 0; for n>1: a(n) = a(n-1) + sin(floor(sqrt(4n-7))·π/2)
 * Range:     i8 n <= 17, i16 n <= 4094, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the in‑grid recurrence by using the cyclotomic pattern of sin. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return 0;

    seq_t x = 0;                /* a(1) = 0 */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* 4i-7 fits in seq_t as long as i is uint64_t */
        seq_t p = (seq_t)i * 4 - 7;      /* 4i-7 */
        seq_t k = floor_sqrt(p);        /* floor(sqrt(4i-7)) */
        seq_t mod = k & 3;              /* k mod 4 */
        seq_t delta = 0;
        if (mod == 1)
            delta = 1;                  /* sin(π/2) = 1   */
        else if (mod == 3)
            delta = -1;                 /* sin(3π/2) = -1 */

        x += delta;                     /* a(i) = a(i-1) + delta */
    }
    return x;
}
