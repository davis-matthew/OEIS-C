/*
 * A101120  Records in A101119, which forms the nonzero differences of A006519
 *          and A003484.
 * https://oeis.org/A101120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3) - 2^((n-1) mod 4) - 8*floor((n+3)/4)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) = 7 */
    if (n < 1) return 0;          /* undefined for n < 1 */

    seq_t total = pow2(n + 3);            /* 2^(n+3) */

    idx_t mod4   = (n - 1) & 3;           /* (n-1) mod 4, 0..3 */
    seq_t sub1   = pow2(mod4);            /* 2^((n-1) mod 4) */

    idx_t flo    = (n + 3) / 4;           /* floor((n+3)/4) */
    seq_t sub2   = (seq_t)8 * flo;        /* 8 * floor((n+3)/4) */

    return total - sub1 - sub2;          /* a(n) */
}
