/*
 * A014840  Sum of all the digits of n in every base prime to n from 2 to n-1.
 * https://oeis.org/A014840
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∑_{b=2}^{n–1, gcd(b,n)=1} digit_sum(n,b)
 * Range:     u8 n <= 36, u16 n <= 598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_sum_in_base, _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_digit_sum_in_base.h"
#include "../utils/_gcd_idx.h"

/* Euclidean algorithm for idx_t */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t total = 0;
    for (idx_t b = 2; b < n; ++b) {
        if (_gcd_idx(n, b) != 1) continue;
        total += _digit_sum_in_base(n, b);
    }
    return total;
}
