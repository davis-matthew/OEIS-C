/*
 * A118277  Generalized 9-gonal (or enneagonal) numbers: m*(7*m - 5)/2 with m
 *          = 0, 1, -1, 2, -2, 3, -3, ...
 * https://oeis.org/A118277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n-5)/2 for all integer values of n (here n is m)
 * Range:     u8 n <= 16, u16 n <= 273, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the unsigned index to a signed 64‑bit integer to handle negative values of m. */
    int64_t i = (int64_t)n;
    int64_t m;

    if (i == 0)
        m = 0;
    else if (i & 1LL)          /* odd index -> positive m = (i+1)/2 */
        m = (i + 1) / 2;
    else                       /* even index -> negative m = -(i/2) */
        m = - (i / 2);

    /* Compute a(n) = m*(7*m-5)/2 with 64‑bit intermediate and cast to seq_t. */
    int64_t num = m * (7 * m - 5);
    int64_t val = num / 2;    /* always integer and non‑negative */

    return (seq_t)val;
}
