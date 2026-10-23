/*
 * A140485  Trajectory of 1 under repeated application of the map: n -> n +
 *          second-smallest number that does not divide n.
 * https://oeis.org/A140485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   second_non_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/second_non_divisor.h"
/* A140485: a(1)=1.
   a(n)=a(n-1)+b, where b is the second‑smallest positive integer that does not divide a(n-1). */

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for 0, fallback */

    seq_t cur = 1;             /* a(1) */
    if (n == 1) return cur;

    /* iterate from 2 up to n, computing each term on the fly */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t inc = second_non_divisor(cur);
        cur += inc;            /* a(i) = a(i-1)+inc */
    }
    return cur;
}
