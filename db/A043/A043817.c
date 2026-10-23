/*
 * A043817  Numbers n such that number of runs in base 3 representation of n
 *          is congruent to 2 mod 10.
 * https://oeis.org/A043817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043817: a(n) = the nth positive integer whose number of runs in its
   base‑3 representation is congruent to 2 modulo 10.
   The function performs a straightforward search; no memoisation is used
   to keep the implementation safe for all integer types given by the build. */

#include <stdint.h>

/* Returns the n-th term of the sequence for n ≥ 1; a(0) is undefined
   and returns 0 as a defensive  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1 – defensive return */

    seq_t x = 1;                           /* candidate number   */
    idx_t found = 0;                       /* how many terms found */

    while (found < n) {
        seq_t y = x;
        int runs  = 0;
        int prev  = -1;

        /* Count runs in the base‑3 representation of x */
        while (y > 0) {
            int d = (int)(y % 3);          /* base‑3 digit   */
            if (d != prev) {
                ++runs;
                prev = d;
            }
            y /= 3;
        }

        if (runs % 10 == 2) {              /* criterion met */
            ++found;
            if (found == n) return x;
        }

        x = x + 1;                          /* next integer   */
    }

    /* Unreachable, but keeps the compiler happy */
    return 0;
}
