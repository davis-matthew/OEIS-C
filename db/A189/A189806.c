/*
 * A189806  Denominators of coefficients in the series expansion of ((2 - m)
 *          EllipticK(m) - 2 EllipticE(m))/(Pi * m).
 * https://oeis.org/A189806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(4*n + 1 - popcount(n-1) - popcount(n+1)) for n≥1,
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;
    idx_t pop1 = __builtin_popcountll((unsigned long long)(n - 1ULL));
    idx_t pop2 = __builtin_popcountll((unsigned long long)(n + 1ULL));
    idx_t exp = 4ULL * n + 1ULL - pop1 - pop2;
    return pow2(exp);
}
