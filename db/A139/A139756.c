/*
 * A139756  Binomial transform of A004526.
 * https://oeis.org/A139756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*2^(n-2) for n>=2, a(0)=0, a(1)=0
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(0) = 0, a(1) = 0 */
    /* For n >= 2: a(n) = (n-1) * 2^(n-2) */
    seq_t factor = (seq_t)(n - 1);
    seq_t p = pow2(n - 2);         /* pow2(exp) returns 2^exp in seq_t */
    return factor * p;
}
