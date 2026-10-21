/*
 * A054848  n - nearest integer to (nearest integer to n^(2/3))^(3/2).
 * https://oeis.org/A054848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n - nearest integer to sqrt( (round(n^(2/3)))^3 ) where
 *            round(x) = nearest integer to x Using integer arithmetic only
 * Range:     i8 n <= 3, i16 n <= 63, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt, pow_seq, round_root2, round_root3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */

#include <stdint.h>
#include "../utils/floor_cube_root.h"
#include "../utils/floor_sqrt.h"
#include "../utils/pow_seq.h"
#include "../utils/round_root2.h"
#include "../utils/round_root3.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;        /* cast index to full precision type */

    /* r1 = round( n^(2/3) )  ≡ round( (n^2)^(1/3) ) */
    seq_t N2 = N * N;
    seq_t r1 = round_root3(N2);

    /* r2 = round( sqrt( r1^3 ) ) */
    seq_t r1_3 = r1 * r1 * r1;
    seq_t r2 = round_root2(r1_3);

    return N - r2;   /* a(n) = n – r2 */
}
