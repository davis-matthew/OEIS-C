/*
 * A017287  a(n) = (10*n + 1)^7.
 * https://oeis.org/A017287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute (10*n + 1) first.  Using seq_t for all arithmetic ensures
       correct type promotion and avoids overflow in intermediate multiplications.
       The formula is (10*n + 1) raised to the 7th power.  pow_seq is a helper
       that efficiently performs repeated multiplication for a small exponent. */
    seq_t base = (seq_t)10 * (seq_t)n + 1;
    return pow_seq(base, 7);
}
