/*
 * A203481  a(n) = v(n+1)/(4*v(n)), where v = A203479.
 * https://oeis.org/A203481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * ∏_{k=1..n} (2^k + 2^(n+1) - 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Product = ∏_{k=1..n} (2^k + 2^(n+1) – 2) */
    seq_t prod = 1;
    seq_t two_n1 = pow2(n + 1);          /* 2^(n+1) */
    seq_t pow2_k = 2;                    /* 2^1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = pow2_k + two_n1 - 2; /* 2^k + 2^(n+1) – 2 */
        prod *= term;
        if (k < n)                       /* prepare next power of 2 */
            pow2_k <<= 1;                /* multiply by 2 */
    }

    prod >>= 2;   /* divide by 4 */
    return prod;
}
