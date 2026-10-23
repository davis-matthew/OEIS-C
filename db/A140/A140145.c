/*
 * A140145  a(1)=1, a(n)=a(n-1)+n^1 if n odd, a(n)=a(n-1)+ n^3 if n is even.
 * https://oeis.org/A140145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n if n is odd a(n-1) + n³ if n is even with a(1)
 *            = 1: a(1)=1 a(2)=1+2³=9 a(3)=9+3=12 a(4)=12+4³=76
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Ensure the offset is respected. For n < 1 behavior is irrelevant. */
    if (n == 0) return 0;

    seq_t result = 1;            /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)i;
        if ((i & 1) == 0) {            /* even */
            term = term * term * term; /* i³ */
        }
        result += term;
    }

    return result;
}
