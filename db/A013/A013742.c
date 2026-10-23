/*
 * A013742  a(n) = 8^(3*n + 1).
 * https://oeis.org/A013742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(3*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = 512^n * 8.
   For arbitrary-precision seq_t it uses the provided utilities
   seq_pow and seq_mul.  For fixed-width builds the usual
   unsigned integer arithmetic is used, with wrap‑around on overflow. */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Base case: n == 0 */
    if (n == 0) {
        return (seq_t)8;
    }

    /* Compute 512^n, then multiply by 8 */
    seq_t pow512 = seq_pow((seq_t)512, n);
    seq_t result = seq_mul(pow512, (seq_t)8);
    return result;
}
