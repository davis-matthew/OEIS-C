/*
 * A004319  a(n) = binomial(3*n, n - 1).
 * https://oeis.org/A004319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n, n - 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 997 (2026-11-01)
 * Helpers:   binomial, gcd_idx_to_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd_idx_to_idx.h"

seq_t a(idx_t n) {
    return binomial(3 * n, n - 1);
}
