/*
 * A231678  a(n) = Sum_{i=0..n} digsum_7(i)^3, where digsum_7(i) = A053828(i).
 * https://oeis.org/A231678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digit_sum_base_7(i))^3
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   digsum_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/digsum_7.h"

/* Returns a(n) for the OEIS sequence A231678. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t d = digsum_7(i);
        seq_t cube = d * d;
        cube *= d;
        total += cube;
    }
    return total;
}
