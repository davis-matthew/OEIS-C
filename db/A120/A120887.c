/*
 * A120887  a(n) is the number of k's in 1..n such that gcd(k,ceiling(n/k)) =
 *          1.
 * https://oeis.org/A120887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k ∈ [1,n] : gcd(k, ceil(n/k)) = 1 }
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
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
        /* Compute ceil(n/k) without overflow */
        idx_t q = n / k;
        idx_t r = n % k;
        idx_t c = q + (r != 0);
        if (gcd((seq_t)k, (seq_t)c) == 1) ++count;
    }
    return count;
}
