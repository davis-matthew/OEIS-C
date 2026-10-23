/*
 * A126449  Row sums of triangle A126445; a(n) = Sum_{k=0..n} C( C(n+2,3) -
 *          C(k+2,3), n-k).
 * https://oeis.org/A126449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C( C(n+2,3) - C(k+2,3), n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline idx_t comb3(idx_t x)
{
    return (idx_t)(( (unsigned __int128)x * (x-1) * (x-2) ) / 6);
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t N = comb3(n + 2);
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t A = N - comb3(k + 2);
        sum += binom(A, n - k);
    }
    return sum;
}
