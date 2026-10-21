/*
 * A053193  Cototient of odd numbers.
 * https://oeis.org/A053193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = cototient(2n+1) = (2n+1) - phi(2n+1)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

/* Main sequence function required by the build system */
seq_t a(idx_t n)
{
    /* 2n+1 is always odd and fits in seq_t because n is idx_t (uint64) */
    seq_t m = (seq_t)n * 2 + 1;
    seq_t phi = calc_phi(m);
    return m - phi;
}
