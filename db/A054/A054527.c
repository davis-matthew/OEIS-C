/*
 * A054527  Triangle read by rows: T(n,k) = Moebius mu(k) (n >= 1, 1 <= k <=
 *          n).
 * https://oeis.org/A054527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 7875, i16 n <= 7875, i32 n <= 7875, i64 n <= 7875,
 *            i128 n <= 7875, big any n
 * Helpers:   int_sqrt, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A054527
   a(n) = μ(k) where k is the column number of the cell that contains
   the index n in the infinite triangle T(n,k)=μ(k) (1≤k≤n).
   R = ceil((√(1+8n)-1)/2) is the row in which the index n lies.
   Then k = n - (R-1)*R/2 and a(n) = μ(k). */

#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Find the row R that contains position n. */
    idx_t sq = int_sqrt(1 + 8 * n);          /* floor sqrt(1+8n)   */
    idx_t R  = (sq - 1) / 2;                 /* floor estimate     */
    if (R * (R + 1) / 2 < n)                 /* adjust to ceil     */
        R++;

    /* Column k inside this row. */
    idx_t prev_tri = (R - 1) * R / 2;        /* T_{R-1}            */
    idx_t k = n - prev_tri;                 /* 1 ≤ k ≤ R          */

    /* Return the Möbius value. */
    return mobius(k);
}
