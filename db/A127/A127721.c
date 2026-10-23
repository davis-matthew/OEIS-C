/*
 * A127721  Floor of square root of sum of squares of n consecutive numbers.
 * https://oeis.org/A127721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(n*(n+1)*(2n+1)/6))
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 566, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* sequence is defined for n ≥ 1 */

    /* Convert index to seq_t to avoid overflow in intermediate multiplications */
    seq_t x = (seq_t)n;
    seq_t y = x + 1;
    seq_t z = (seq_t)2 * x + 1;     /* 2n+1 */

    /* cancel a factor of 2 */
    if ((x & 1) == 0)
        x >>= 1;                    /* x /= 2 */
    else
        y >>= 1;                    /* y /= 2 */

    /* cancel a factor of 3 */
    if (x % 3 == 0)
        x /= 3;
    else if (y % 3 == 0)
        y /= 3;
    else
        z /= 3;                     /* exactly one of x,y,z is divisible by 3 */

    seq_t prod = x * y;
    prod *= z;                      /* prod = n*(n+1)*(2n+1)/6 */

    return sqrt_floor(prod);        /* floor of the square root */
}
