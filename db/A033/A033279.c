/*
 * A033279  Number of diagonal dissections of an n-gon into 7 regions.
 * https://oeis.org/A033279
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,6) * binomial(n-3,6) / 7
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 21, u64 n <= 143,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;                 /* offset starts at 8 */

    seq_t b1 = binomial(n + 5, 6);
    seq_t b2 = binomial(n - 3, 6);

    /* Reduce the 7‐division early to avoid overflow in intermediate result */
    seq_t g     = gcd(b1, (seq_t)7);
    seq_t denom = (seq_t)7 / g;
    b1 /= g;

    seq_t res = b1 * b2;
    res /= denom;
    return res;
}
