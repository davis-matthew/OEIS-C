/*
 * A231680  a(n) = Sum_{i=0..n} digsum_8(i), where digsum_8(i) = A053829(i).
 * https://oeis.org/A231680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} A053829(i) where A053829(i) = sum of the
 *            base‑8 digits of i
 * Range:     u8 n <= 44, u16 n <= 4836, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_sum_base8.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i)
        total += digit_sum_base8(i);
    return total;
}
