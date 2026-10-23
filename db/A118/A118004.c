/*
 * A118004  a(n) = 9^n - 4^n.
 * https://oeis.org/A118004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute 9^n and 4^n using the provided pow_seq routine
       (exponent fits in a signed 32‑bit int for n ≤ 10000). */
    int exp = (int)n;
    seq_t pow9 = pow_seq((seq_t)9, exp);
    seq_t pow4 = pow_seq((seq_t)4, exp);
    return pow9 - pow4;
}
