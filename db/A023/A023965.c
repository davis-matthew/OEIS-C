/*
 * A023965  First digit after decimal point of 6th root of n.
 * https://oeis.org/A023965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10*n^(1/6)) mod 10
 * Range:     u8 none, u16 none, u32 n <= 1001, u64 n <= 1001, u128 n <= 1001,
 *            big any n
 * Helpers:   integer_sixth_root, power6_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/integer_sixth_root.h"
#include "../utils/power6_idx.h"

seq_t a(idx_t n)
{
    /* Scale by 10^6 to capture the first decimal digit. */
    seq_t scaled = (seq_t)n * 1000000ULL;
    idx_t root = integer_sixth_root(scaled);
    return (seq_t)(root % 10);
}
