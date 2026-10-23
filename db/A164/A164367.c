/*
 * A164367  a(n) = A164051(n) in base 2.
 * https://oeis.org/A164367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n) + 10^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1 */
    idx_t e1 = n << 1;            /* 2*n, safe for n up to 2^63-1 */
    idx_t e2 = n - 1;
    seq_t t1 = pow10(e1);         /* 10^(2*n) */
    seq_t t2 = pow10(e2);         /* 10^(n-1) */
    return t1 + t2;
}
