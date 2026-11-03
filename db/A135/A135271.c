/*
 * A135271  Row 7 of A038207.
 * https://oeis.org/A135271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, nonn, full
 * Formula:   a(n) = binom(7, n) * 2^(7-n)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: full, pass, 8 terms through n = 7 (2026-11-02)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n > 7) return 0;
    return binomial(7, n) * pow2(7 - n);
}
