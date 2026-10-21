/*
 * A227978  a(0)=1, a(1)=2; for n>1, a(n) = n*(2^n+4)/4.
 * https://oeis.org/A227978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^n+4)/4 for n > 1; a(0) = 1, a(1) = 2. Equivalent to
 *            a(n) = n*(2^(n-2)+1) for n >= 2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* For n >= 2: use a(n) = n * (2^(n-2) + 1) */
    seq_t pow = pow2(n - 2);       /* computes 2^(n-2) */
    return (seq_t)n * (pow + 1);
}
