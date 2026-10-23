/*
 * A034841  a(n) = (n^2)! / (n!)^n.
 * https://oeis.org/A034841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} binomial(k*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
        result = result * binomial(k * n, n);

    return result;
}
