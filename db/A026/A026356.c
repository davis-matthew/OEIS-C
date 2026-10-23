/*
 * A026356  a(n) = floor((n-1)*phi) + n + 1, n > 0, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A026356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1)*phi) + n + 1
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* n should be >= 1; return 0 for safety */
    seq_t m = (seq_t)(n - 1);        /* m = n-1 */
    seq_t t = (seq_t)5 * m * m;      /* t = 5*m*m = 5*(n-1)^2 */
    seq_t sqrt_t = floor_sqrt(t);    /* sqrt_t = floor((n-1)*sqrt(5)) */
    seq_t part = (m + sqrt_t) / 2;   /* floor((n-1)*phi) = (m + sqrt_t)/2 */
    seq_t result = part + m + 2;     /* a(n) = floor((n-1)*phi) + n + 1 = part + m + 2 */
    return result;
}
