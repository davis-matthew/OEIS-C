/*
 * A056011  Enumeration of natural numbers by the boustrophedonic diagonal
 *          method.
 * https://oeis.org/A056011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) = ((i+j-1)*(i+j-2)+((-1)^t+1)*i - ((-1)^t-1)*j)/2 where i =
 *            n - t*(t+1)/2, j = (t*t + 3*t + 4)/2 - n, t =
 *            floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 32, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   floor_sqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt_seq.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* t = floor((-1+sqrt(8*n-7))/2) */
    seq_t tmp = (seq_t)8 * N - (seq_t)7;
    seq_t s   = floor_sqrt_seq(tmp);     /* floor(sqrt(tmp)) */
    seq_t t   = (s - (seq_t)1) / (seq_t)2; /* integer division */

    /* i = n - t*(t+1)/2 */
    seq_t tri_t = t * (t + (seq_t)1) / (seq_t)2;
    seq_t i = N - tri_t;

    /* j = (t*t + 3*t + 4)/2 - n */
    seq_t j = ((t * t) + (seq_t)3 * t + (seq_t)4) / (seq_t)2 - N;

    /* numerator part (i+j-1)*(i+j-2) */
    seq_t sum = i + j;
    seq_t num = (sum - (seq_t)1) * (sum - (seq_t)2);

    /* add parity-dependent term */
    if ((t & (seq_t)1) == (seq_t)0)     /* t even */
        num += (seq_t)2 * i;
    else                                 /* t odd  */
        num += (seq_t)2 * j;

    /* final division */
    return num / (seq_t)2;
}
