/*
 * A135427  a(n) = 8^n * 9^(n^2).
 * https://oeis.org/A135427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n * 9^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Compute 8^n */
    seq_t eight = (seq_t)8;
    seq_t pow8 = pow_seq(eight, (int)n);

    /* Compute 9^(n^2) */
    seq_t nine = (seq_t)9;
    /* n^2 fits in 32‑bit for n <= 10000 */
    idx_t n2 = n * n;
    seq_t pow9 = pow_seq(nine, (int)n2);

    return pow8 * pow9;
}
