/*
 * A160926  a(n)= n * reversal(n+1).
 * https://oeis.org/A160926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * rev(n+1)
 * Range:     u8 n <= 11, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 859 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    idx_t r = reverse_digits_idx(n + 1);
    return (seq_t)n * (seq_t)r;
}
