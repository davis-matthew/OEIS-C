/*
 * A211377  T(n,k) = ((k + n)^2 - 4*k + 3 + (-1)^k - (k + n - 2)*(-1)^(k +
 *          n))/2; n, k > 0, read by antidiagonals.
 * https://oeis.org/A211377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((t + 2)^2 - 4*j + 3 + (-1)^j - t*(-1)^t)/2 where t =
 *            floor((sqrt(8*n - 7) - 1)/2) j = ((t*t + 3*t + 4)/2) - n
 * Range:     u8 n <= 8, u16 n <= 925, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* offset starts at 1 */

    /* Compute t = floor((sqrt(8*n - 7) - 1)/2) */
    seq_t tmp = (seq_t)n * (seq_t)8 - (seq_t)7;
    seq_t s   = floor_sqrt(tmp);
    seq_t t   = (s - (seq_t)1) / (seq_t)2;

    /* Compute j = ((t*t + 3*t + 4)/2) - n */
    seq_t j   = ((t * t + (seq_t)3 * t + (seq_t)4) / (seq_t)2) - n;

    /* Numerator: (t+2)^2 + 3 + (-1)^j + (-t*(-1)^t) - 4*j */
    seq_t N = (t + (seq_t)2) * (t + (seq_t)2); /* (t+2)^2 */
    N += (seq_t)3;                              /* +3 */

    /* (+/- 1) from (-1)^j */
    if ((j & (seq_t)1) != (seq_t)0)  /* j odd */ N -= (seq_t)1;
    else                            /* j even */ N += (seq_t)1;

    /* (+/- t) from -t*(-1)^t */
    if ((t & (seq_t)1) != (seq_t)0)  /* t odd  */ N += t;
    else                            /* t even */ N -= t;

    /* Subtract 4*j */
    N -= (seq_t)4 * j;

    /* Divide by 2 (resulting integer) */
    return N / (seq_t)2;
}
