/*
 * A131679  a(n) = (Product_{i=1..9} n^i+i) / 9!.
 * https://oeis.org/A131679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{i=1..9} (n^i + i)) / 9!
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 3, u128 n <= 9,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    const seq_t denom_init = (seq_t)362880;   /* 9!  */
    seq_t denom = denom_init;
    seq_t power = (seq_t)1;
    seq_t ans   = (seq_t)1;

    for (idx_t i = 1; i <= 9; ++i) {
        power = power * (seq_t)n;            /* power = n^i   */
        seq_t factor = power + (seq_t)i;     /* factor = n^i + i */

        seq_t g = gcd(factor, denom);        /* cancel common factors */
        if (g != (seq_t)1) {
            factor /= g;
            denom  /= g;
        }
        ans *= factor;
    }

    if (denom > (seq_t)1)
        ans /= denom;                         /* safety: should be 1 */

    return ans;
}
