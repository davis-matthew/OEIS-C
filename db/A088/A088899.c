/*
 * A088899  T(n, k) = number of ordered pairs of integers (x,y) with x^2/n^2 +
 *          y^2/k^2 = 1, 1 <= k <= n; triangular array, read by rows.
 * https://oeis.org/A088899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1411, u32 n <= 8512, u64 n <= 8512,
 *            u128 n <= 8512, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the term a(n) of OEIS sequence A088899.
   a(n) is the row‑wise reading of the triangular array T(r,k), where
   T(r,k) = number of lattice points (x,y) satisfying
            x^2/r^2 + y^2/k^2 = 1.  For the global index n (offset 1)
   we locate the corresponding row r and column k, and then count
   all integer solutions (x,y).  The calculation works for any
   seq_t defined in the build and uses only integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range indices */

    /* Find the row r such that r*(r+1)/2 >= n (rows are 1‑based). */
    seq_t r = 1;
    while (1) {
        seq_t tri = (seq_t)r * (r + 1) / 2;
        if (tri >= n) break;
        r++;
    }
    seq_t prev_tri = (seq_t)(r - 1) * r / 2;
    seq_t k = n - prev_tri;          /* column index (1‑based) */

    /* Count integer solutions to x^2 * k^2 + y^2 * r^2 = (r*k)^2. */
    seq_t count = 0;
    seq_t R2 = (seq_t)r * r;
    seq_t k2 = (seq_t)k * k;

    for (int64_t x = -(int64_t)r; x <= (int64_t)r; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        if (x2 > R2) continue;                /* safety, should not happen */

        seq_t num = k2 * (R2 - x2);            /* numerator of y² */
        if (num % R2 != 0) continue;           /* y² not integer */

        seq_t y2 = num / R2;
        seq_t y  = isqrt_seq(y2);
        if (y * y != y2) continue;             /* not a perfect square */

        if (y == 0) {
            /* Only one ordered pair (x,0) for this x. */
            count += 1;
        } else {
            /* Two signs of y. */
            count += 2;
        }
    }
    return count;
}
