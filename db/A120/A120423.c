/*
 * A120423  a(n) = maximum value among all k where 1<=k<=n of
 *          gcd(k,floor(n/k)).
 * https://oeis.org/A120423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max_{1 <= k <= n} gcd(k, floor(n/k))
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t max_gcd = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t fk = (seq_t)n / (seq_t)k;     /* floor(n/k) */
        seq_t g = gcd((seq_t)k, fk);
        if (g > max_gcd) {
            max_gcd = g;
        }
    }
    return max_gcd;
}
