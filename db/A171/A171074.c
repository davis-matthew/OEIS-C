/*
 * A171074  A115112 with initial term changed from 0 to 1.
 * https://oeis.org/A171074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; a(n) = binom(2*n, n) - 2 for n >= 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (so a(1) is the first term listed below) */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    seq_t val = binomial(n << 1, n);
    val -= 2;
    return val;
}
