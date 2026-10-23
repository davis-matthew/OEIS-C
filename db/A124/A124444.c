/*
 * A124444  a(n) = LCM of the integers, from n/2 to n, which are coprime to n.
 * https://oeis.org/A124444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM of the integers k with ceil(n/2) ≤ k ≤ n that are
 *            coprime to n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t start = (n + 1) / 2;   /* ceil(n/2) */
    seq_t result = 1;

    for (idx_t k = start; k <= n; ++k) {
        if (gcd((seq_t)k, (seq_t)n) == 1) {
            seq_t g = gcd(result, (seq_t)k);
            result = (result / g) * (seq_t)k;
        }
    }
    return result;
}
