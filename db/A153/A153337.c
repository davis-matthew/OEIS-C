/*
 * A153337  Number of zig-zag paths from top to bottom of a 2n-1 by 2n-1
 *          square whose color is that of the top right corner.
 * https://oeis.org/A153337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^(2n-2) - 2*(n-1)*C(2n-2,n-1) (OEIS A153337)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 999 (2026-11-01)
 * Helpers:   binomial, bit_length, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/bit_length.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* Handle the base case n = 1 explicitly to avoid negative indices */
    if (n == 1) return (seq_t)1;

    /* Compute 2^(2n-2) */
    idx_t exp = 2*n - 2;                 /* exponent for the power of two */
    seq_t pow_two = ipow_u64_idx(2, exp);    /* 2^exp */

    /* Term 1: n * 2^(2n-2) */
    seq_t term1 = ((seq_t)n) * pow_two;

    /* Term 2: 2*(n-1)*C(2n-2,n-1) */
    seq_t comb = binomial(2*n - 2, n - 1);
    seq_t term2 = 2 * ((seq_t)(n - 1)) * comb;

    /* Result */
    return term1 - term2;
}
