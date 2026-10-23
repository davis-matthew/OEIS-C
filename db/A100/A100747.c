/*
 * A100747  A modular recurrence.
 * https://oeis.org/A100747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*n) = 15^n, a(2*n+1) = 3*15^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* Compute the nth term of OEIS A100747 */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                  /* n is even => a(n) = 15^(n/2) */
        return pow_seq((seq_t)15, n >> 1);
    } else {                              /* n is odd  => a(n) = 3*15^((n-1)/2) */
        seq_t val = pow_seq((seq_t)15, n >> 1);
        return val * (seq_t)3;
    }
}
