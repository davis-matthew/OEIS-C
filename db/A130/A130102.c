/*
 * A130102  E.g.f.: (e^x - x)^2.
 * https://oeis.org/A130102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n==2?2:2^n - 2*n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility pow2(idx_t exp) from ../utils/pow2.h */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)2;          /* special case a(2) = 2 */

    /* compute 2^n using the provided pow2 helper */
    seq_t pow2n = pow2(n);

    /* compute 2*n as seq_t */
    seq_t two_times_n = (seq_t)2 * (seq_t)n;

    /* a(n) = 2^n - 2*n */
    return pow2n - two_times_n;
}
