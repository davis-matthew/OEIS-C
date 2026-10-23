/*
 * A182398  a(n) = (Sum_{k=1..2n} k^2n) mod 2n.
 * https://oeis.org/A182398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{k=1}^{2n} k^{2n}) mod 2n
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return 0;

    idx_t m   = 2 * n;               /* modulus */
    seq_t mod = (seq_t)m;
    seq_t sum = 0;

    for (idx_t k = 1; k <= m; ++k)
    {
        /* compute k^m (mod m) */
        seq_t term = mod_pow((seq_t)k, m, mod);

        sum += term;
        if (sum >= mod)
            sum -= mod;              /* keep sum < 2*mod to stay in bounds */
    }

    return sum % mod;
}
