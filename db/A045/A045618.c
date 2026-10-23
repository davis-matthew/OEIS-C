/*
 * A045618  Partial sums of A000337(n+4), n >= 0.
 * https://oeis.org/A045618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n + 5 + (n-1)*2^(n+2) for n >= 0, with a(0)=1 * Uses the
 *            provided pow2(idx_t) helper to compute 2^(n+2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* 2^(n+2) as seq_t */
    seq_t pow = pow2(n + 2);
    /* Compute (n-1) as seq_t; n>=1 so subtraction is safe */
    seq_t mult = ((seq_t)(n - 1)) * pow;
    /* Final result: n + 5 + (n-1)*2^(n+2) */
    return (seq_t)n + (seq_t)5 + mult;
}
