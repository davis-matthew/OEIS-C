/*
 * A237525  Numbers k such that the sum of digits of k^3 is a cube.
 * https://oeis.org/A237525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 159, u64 n <= 9640,
 *            u128 n <= 9640, big any n
 * Helpers:   digit_sum, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: returns the n‑th integer k such that
   the sum of decimal digits of k³ is a perfect cube.  The search starts
   from 0 and proceeds incrementally until the n‑th qualifying k is found. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* The sequence is indexed from 1.  We assume n ≥ 1. */
    if (n == 0) abort();        /* invalid request – enforce offset 1 */

    idx_t k = 0;                /* candidate integer */
    idx_t found = 0;            /* count of qualifying terms */

    for (;; ++k) {
        seq_t kk = (seq_t)k;          /* promote to seq_t for arithmetic */
        seq_t k3 = kk * kk * kk;      /* k³ */
        seq_t sum = digit_sum(k3);    /* sum of decimal digits of k³ */
        seq_t c   = floor_cube_root(sum);
        if (c * c * c == sum) {      /* sum is a perfect cube */
            ++found;
            if (found == n)
                return kk;           /* nth term found */
        }
    }
}
