/*
 * A051190  a(n) = Product_{k=1..n-1} gcd(k,n).
 * https://oeis.org/A051190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = ∏_{k=1}^{n-1} gcd(k,n) (OEIS A051190)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 29, u64 n <= 47, u128 n <= 83,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t prod = 1;
    seq_t sn = (seq_t)n;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t g = gcd((seq_t)k, sn);
        prod = prod * g;
    }
    return prod;
}
