/*
 * A120425  a(n) = maximum value among all k where 1<=k<=n of
 *          GCD(k,ceiling(n/k)).
 * https://oeis.org/A120425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max_{1≤k≤n} gcd(k, ceil(n/k))
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* safety; sequence is defined for n ≥ 1 */
        return 0;

    seq_t best = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = k;
        seq_t y  = ((seq_t)n + kk - 1) / kk;   /* ceil(n/k) */
        seq_t g  = gcd(kk, y);
        if (g > best)
            best = g;
    }
    return best;
}
