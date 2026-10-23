/*
 * A103604  a(n) = binomial(n+6,6) * binomial(n+10,6).
 * https://oeis.org/A103604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) * binomial(n+10,6)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 13, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b1 = binomial(n + 6, 6);
    seq_t b2 = binomial(n + 10, 6);
    return b1 * b2;
}
