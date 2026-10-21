/*
 * A238604  a(n) = Sum_{k=0..3} f(n+k)^2 where f=A130519.
 * https://oeis.org/A238604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f(n)^2 + f(n+1)^2 + f(n+2)^2 + f(n+3)^2 where f(n) =
 *            A130519(n) For n < 4, f(n) = 0. For n ≥ 4, let j = (n-4)/4, r =
 *            (n-4)%4 then f(n) = (j+1) * (2*j + 1 + r). This follows the
 *            definition of A130519
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   fA130519
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fA130519.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < 4; ++k)
    {
        seq_t f = fA130519(n + k);
        sum += f * f;
    }
    return sum;
}
