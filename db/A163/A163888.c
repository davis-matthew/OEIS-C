/*
 * A163888  a(n) = 2*a(n-2) for n > 2; a(1) = 5, a(2) = 4.
 * https://oeis.org/A163888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7 - 3*(-1)^n) * 2^((2*n-5+(-1)^n)/4)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)4;
    /* For n > 2, a(n) = 2 * a(n-2).  Compute directly using powers of two. */
    if ((n & 1) == 1) {                /* n is odd: a(n) = 5 * 2^((n-1)/2) */
        return (seq_t)5 * pow2((n - 1) / 2);
    } else {                           /* n is even: a(n) = 4 * 2^((n-2)/2) */
        return (seq_t)4 * pow2((n - 2) / 2);
    }
}
