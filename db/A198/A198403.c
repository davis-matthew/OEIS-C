/*
 * A198403  a(n) = 6^n * n^6.
 * https://oeis.org/A198403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * n^6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    /* compute 6^n   (big_pow works for all seq_t types) */
    seq_t base6 = 6;
    seq_t pow6   = big_pow(base6, (seq_t)n);

    /* compute n^6   (pow_seq works for all seq_t types) */
    seq_t n6 = pow_seq((seq_t)n, 6);

    /* final product */
    return seq_mul(pow6, n6);
}
