/*
 * A109172  Recursive form of A109845 without a(2)=3.
 * https://oeis.org/A109172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + (-1)^n*a(n-1)+(-1)^n with a(1)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterative recurrence implementation. */
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; undefined for 0 */
    seq_t val = (seq_t)2;
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & (idx_t)1) == 0) {
            val = val * val + val + (seq_t)1;
        } else {
            val = val * val - val - (seq_t)1;
        }
    }
    return val;
}
