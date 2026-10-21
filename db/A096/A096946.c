/*
 * A096946  Ninth column of (1,5)-Pascal triangle A096940.
 * https://oeis.org/A096946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+40)*binomial(n+7, 7)/8
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Compute (n+40) * C(n+7, 7) / 8 using the provided binomial helper. */
    return ((seq_t)(n + 40) * binomial(n + 7, 7)) / 8;
}
