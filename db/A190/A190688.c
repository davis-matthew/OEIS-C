/*
 * A190688  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),3,2) and [ ]=floor.
 * https://oeis.org/A190688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((bn+c)r) - b*floor(nr) - floor(cr) with r =
 *            sqrt(3), b = 3, c = 2
 * Range:     u8 none, u16 n <= 9, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* compute floor(k * sqrt(3)) as floor(sqrt(3 * k^2))                     */
    seq_t k1    = (seq_t)3 * n + 2;                 /* k1 = 3n + 2 */
    seq_t k1sq  = k1 * k1;                          /* k1^2    */
    seq_t s1    = sqrt_floor((seq_t)3 * k1sq);     /* floor(sqrt(3*k1^2)) */

    seq_t n_sq  = (seq_t)n * (seq_t)n;              /* n^2    */
    seq_t s2    = sqrt_floor((seq_t)3 * n_sq);      /* floor(sqrt(3*n^2)) */

    seq_t result = s1 - 3 * s2 - 3;                 /* a(n) */
    return result;
}
