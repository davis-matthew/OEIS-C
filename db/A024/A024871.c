/*
 * A024871  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2 ], s =
 *          (natural numbers >= 2), t = A023532.
 * https://oeis.org/A024871
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊n/2⌋} (i+1) * t(n-i+1), where t(m) = 0 if m+1
 *            is a triangular number, otherwise t(m) = 1
 * Range:     u8 n <= 45, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_zero_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_zero_t.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t k = n / 2;
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t j = n - i + 1;
        bool zero = is_zero_t(j);
        sum += (seq_t)(i + 1) * (zero ? 0 : 1);
    }
    return sum;
}
