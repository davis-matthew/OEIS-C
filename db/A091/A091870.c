/*
 * A091870  A trinomial transform of Fibonacci(3n).
 * https://oeis.org/A091870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t a0 = 0;   /* a(0) */
    seq_t a1 = 1;   /* a(1) */

    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t ai, t1, t2;
    for (idx_t i = 2; i <= n; ++i) {
        t1 = mul_small(a1, 8);
        t2 = mul_small(a0, 11);
        ai = t1 - t2;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
