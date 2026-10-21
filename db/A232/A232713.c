/*
 * A232713  Doubly pentagonal numbers: a(n) = n*(3*n-2)*(3*n-1)*(3*n+1)/8.
 * https://oeis.org/A232713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n-2)*(3*n-1)*(3*n+1)/8
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 189, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the offset directly to avoid underflow for n = 0 */
    if (n == 0)
        return 0;

    /* Cast index to the value type early to keep all arithmetic in
       the same type. */
    seq_t n_t = (seq_t)n;
    seq_t f0  = n_t;               /* n */
    seq_t f1  = (seq_t)(3 * n_t - 2); /* 3n - 2 */
    seq_t f2  = (seq_t)(3 * n_t - 1); /* 3n - 1 */
    seq_t f3  = (seq_t)(3 * n_t + 1); /* 3n + 1 */

    /* Reduce the factor 8 across the four multiplicands.  Since the
       product is divisible by 8, this division is exact. */
    seq_t d = 8, g;
    g = gcd(f0, d);  f0 /= g;  d /= g;
    g = gcd(f1, d);  f1 /= g;  d /= g;
    g = gcd(f2, d);  f2 /= g;  d /= g;
    g = gcd(f3, d);  f3 /= g;  d /= g;   /* After this d == 1 */

    /* Multiply the reduced factors; the intermediate results never
       exceed the final value, so no overflow occurs for all
       n where the answer fits in seq_t. */
    seq_t res = f0;
    res = res * f1;
    res = res * f2;
    res = res * f3;

    return res;
}
