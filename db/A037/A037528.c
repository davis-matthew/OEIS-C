/*
 * A037528  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,2.
 * https://oeis.org/A037528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(7*3^n/13) (OEIS A037528)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1.  For safety, return 0 when n==0. */
    if (n == 0) return (seq_t)0;

    /* Compute 3^n as a big integer. */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);

    /* Compute floor(7*pow3/13).  All intermediate operations are
       performed in the seq_t type, so the result is exact.  For fixed
       width integer types this relies on the fact that 7*3^n fits in
       seq_t for all n that are expected to be tested. */
    return (pow3 * (seq_t)7) / (seq_t)13;
}
