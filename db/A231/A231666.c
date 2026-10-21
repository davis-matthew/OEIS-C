/*
 * A231666  a(n) = Sum_{i=0..n} digsum_4(i)^3, where digsum_4(i) = A053737(i).
 * https://oeis.org/A231666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_4(i)^3, where digsum_4(i) is the *
 *            sum of the base-4 digits of i
 * Range:     u8 n <= 10, u16 n <= 238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum_base4.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t d = digit_sum_base4(i);
        seq_t cube = d * d * d;
        total += cube;
    }
    return total;
}
