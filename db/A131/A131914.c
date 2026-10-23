/*
 * A131914  3*A002024 - 2*A051340.
 * https://oeis.org/A131914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*A002024 - 2*A051340. Equivalent closed form: let t =
 *            floor((sqrt(8*n-7)-1)/2); then a(n) = (t+1)*(t+3) - 2*n
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute t = floor((sqrt(8*n-7)-1)/2) */
    seq_t val = (seq_t)n * (seq_t)8 - (seq_t)7;
    seq_t s   = floor_sqrt(val);       /* floor sqrt of 8*n-7 */
    seq_t t   = (s - (seq_t)1) / (seq_t)2;

    /* a(n) = (t+1)*(t+3) - 2*n */
    seq_t term1 = (t + (seq_t)1) * (t + (seq_t)3);
    seq_t term2 = (seq_t)2 * (seq_t)n;
    return term1 - term2;
}
