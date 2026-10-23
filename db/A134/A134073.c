/*
 * A134073  Complete list of solutions to y^2 = x^3 + 73; sequence gives y
 *          values.
 * https://oeis.org/A134073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 6, u128 n <= 6, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for all integer solutions of
   y^2 = x^3 + 73.  The curve has only six integer points with
   positive y (OEIS A134073).  The algorithm enumerates x from
   –4 to a modest upper bound (2000), tests whether x^3+73 is a
   perfect square, and stores the positive square roots.  After
   initialization the array is indexed directly.  If n exceeds the
   number of solutions the function returns 0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static table of solutions, populated once */
    static seq_t solutions[32];
    static size_t solcnt = 0;
    static bool initialized = false;

    if (!initialized) {
        const int MAX_X = 2000;   /* safe upper bound for this curve */
        for (int x = -4; x <= MAX_X; ++x) {
            /* Compute x^3 + 73 in signed 64‑bit to avoid overflow */
            long long cube = (long long)x * x * x;
            long long val  = cube + 73;
            if (val < 0) continue;               /* no square possible */
            seq_t s = (seq_t)val;                /* s >= 0 */
            seq_t y = floor_sqrt(s);
            if (y * y == s) {
                if (solcnt < 32) {
                    solutions[solcnt++] = y;
                }
            }
        }
        initialized = true;
    }

    if (n == 0 || (size_t)(n-1) >= solcnt) return (seq_t)0;
    return solutions[(size_t)(n-1)];
}
