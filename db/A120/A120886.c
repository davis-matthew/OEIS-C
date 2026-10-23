/*
 * A120886  a(n) = number of k's with 1 <= k <= n where gcd(k,ceiling(n/k)) >
 *          1.
 * https://oeis.org/A120886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k (1 <= k <= n) such that gcd(k, ceil(n/k)) > 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = (n + k - 1) / k;          /* ceil(n/k) */
        if (gcd((seq_t)k, (seq_t)t) > 1)
            ++count;
    }
    return count;
}
