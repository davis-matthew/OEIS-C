/*
 * A152020  Denominator of 8/(9*n^2) divided by 9.
 * https://oeis.org/A152020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2/gcd(n^2,8)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out-of-range calls */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t g  = gcd(n2, (seq_t)8);  /* gcd(n^2,8) */
    return n2 / g;
}
