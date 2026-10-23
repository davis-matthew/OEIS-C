/*
 * A131970  1 followed by 2n 2's.
 * https://oeis.org/A131970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^(2*n)-1)/9 + 10^(2*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^(2*n) */
    idx_t e = 2 * n;
    seq_t p = pow10(e);          /* p = 10^(2*n) */

    /* Compute 2*(10^(2*n)-1)/9 */
    seq_t t = p - (seq_t)1;      /* t = 10^(2*n) - 1 */
    t = t / (seq_t)9;            /* t = (10^(2*n)-1)/9 */
    t = t * (seq_t)2;            /* t = 2*(10^(2*n)-1)/9 */

    /* Sum the two parts */
    return p + t;                /* a(n) = 10^(2*n) + 2*(10^(2*n)-1)/9 */
}
