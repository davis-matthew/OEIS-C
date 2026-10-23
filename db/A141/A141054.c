/*
 * A141054  8-idempotent numbers: a(n) = binomial(n+8,8)*8^n.
 * https://oeis.org/A141054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * 8^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute binomial(n+8,8) = product_{i=1}^{8} (n+i)/i */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= 8; ++i) {
        bin = bin * (seq_t)(n + i) / (seq_t)i;
    }

    /* compute 8^n using the arbitrary‑precision power helper */
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);

    /* final result */
    return bin * pow8;
}
