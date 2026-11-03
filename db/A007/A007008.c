/*
 * A007008  Chvatal conjecture for radius of graph of maximal intersecting
 *          sets.
 * https://oeis.org/A007008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(2^(n-2) - binomial(n-1, floor((n-1)/2))/2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t pow2_val = pow2(n - 1);
    seq_t binom_val = binomial(n - 1, (n - 1) / 2);
    return (pow2_val - binom_val + 1) / 2;
}
