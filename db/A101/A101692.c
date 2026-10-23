/*
 * A101692  A modular binomial sum transform of 2^n.
 * https://oeis.org/A101692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} (binomial(2*n+2 , k) mod 2) * 2^k
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 46, u64 n <= 94, u128 n <= 190,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent bit condition: add 2^k iff (k & (2*n+2 - k)) == 0 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t N = (n << 1) + 2;   /* 2*n + 2 */

    for (idx_t k = 0; k <= n; ++k) {
        if ( (k & (N - k)) == 0 ) {
            result |= pow2(k);
        }
    }

    return result;
}
