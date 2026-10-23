/*
 * A140158  a(1)=1, a(n) = a(n-1) + n^4 if n odd, a(n) = a(n-1) + n^1 if n is
 *          even.
 * https://oeis.org/A140158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n^4 if n is odd, a(n-1) + n if n is even, with
 *            a(1)=1
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = (seq_t)i;
        if (i & 1) {              /* odd i */
            sum += x * x * x * x; /* i^4 */
        } else {                  /* even i */
            sum += x;             /* i^1 */
        }
    }
    return sum;
}
