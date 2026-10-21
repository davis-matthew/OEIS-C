/*
 * A061229  a(n) = floor of the geometric mean of n and the reversal of n.
 * https://oeis.org/A061229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(sqrt(n * rev(n)))
 * Range:     u8 n <= 7, u16 n <= 425, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   floor_sqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t reverse_n = rev(n);
    seq_t product = (seq_t)n * (seq_t)reverse_n;
    return floor_sqrt(product);
}
