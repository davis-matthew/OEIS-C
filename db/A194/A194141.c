/*
 * A194141  Sum{floor(j*(2+sqrt(3))) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for 2+sqrt(3).
 * https://oeis.org/A194141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1}^{n} floor(j*(2+sqrt(3)))
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The Beatty sequence term for j is
         floor(j*(2+sqrt(3))) = 2*j + floor(j*sqrt(3))
       and floor(j*sqrt(3)) = floor( sqrt(3*j*j) ).
       We sum 2*j over j in closed form and add the remainder.    */
    seq_t sum = (seq_t)n * ((seq_t)n + 1);          /* sum of 2*j = 2 * n(n+1)/2 */
    for (idx_t j = 1; j <= n; ++j) {
        seq_t jj = (seq_t)j * j;                  /* j*j */
        seq_t sq = 3 * jj;                        /* 3*j*j */
        seq_t fsq = floor_sqrt(sq);              /* floor(sqrt(3*j*j)) */
        sum += fsq;                               /* add floor(j*sqrt(3)) */
    }
    return sum;
}
