/*
 * A051844  a(n) = LCM_{k=0..n} (2^k + 1).
 * https://oeis.org/A051844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{k=0..n} (2^k + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   gcd_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_local.h"

seq_t a(idx_t n)
{
    seq_t lcm  = 1;  /* current LCM, starts with 1 */
    seq_t pow2 = 1;  /* 2^k for the current k, starts with 2^0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = pow2 + 1;          /* 2^k + 1 */
        seq_t g    = gcd_local(lcm, term);
        lcm        = (lcm / g) * term;  /* update LCM */
        pow2       = pow2 * 2;          /* prepare next power of two */
    }

    return lcm;
}
