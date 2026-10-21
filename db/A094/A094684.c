/*
 * A094684  Records in A094683.
 * https://oeis.org/A094684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(max(2*n-1,0)^(3/2))
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 840, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert n to seq_t before multiplication to keep overflow in seq_t domain */
    seq_t x = (seq_t)n * 2 - 1;   /* x = 2*n-1, for n>=1 this is >=1 */
    seq_t x2 = x * x;             /* x^2 */
    seq_t x3 = x2 * x;            /* x^3 */

    return floor_sqrt(x3);        /* floor(sqrt(x^3)) = floor(x^(3/2)) */
}
