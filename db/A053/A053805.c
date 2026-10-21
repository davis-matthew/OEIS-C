/*
 * A053805  Expansion of (1 + x)^12 / (1 - x)^13.
 * https://oeis.org/A053805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..12} binom(12,k) * binom(12 + n - k, n - k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 106, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_k = (n < (idx_t)12) ? n : (idx_t)12;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = binom((idx_t)12, k);
        term *= binom((12 + n - k), n - k);
        result += term;
    }
    return result;
}
