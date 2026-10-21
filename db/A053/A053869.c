/*
 * A053869  Sum of divisors of n less than n is even.
 * https://oeis.org/A053869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th integer m for which the sum of proper divisors
 *            of m is even. Equivalent condition: - m is an odd perfect
 *            square, or - m is even and is neither a square nor twice a
 *            square. This avoids any floating‑point arithmetic and runs in
 *            O(n)
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    idx_t m = 1;                 /* candidate number */
    idx_t count = 0;             /* how many valid numbers found */

    while (1) {
        int include = 0;         /* set to 1 when m satisfies the criteria */

        if (m & 1) { /* odd m */
            idx_t s = _floor_isqrt_u64(m);
            include = (s * s == m);              /* odd perfect square? */
        } else { /* even m */
            idx_t s = _floor_isqrt_u64(m);
            if (s * s == m)                     /* exclude even squares */
                include = 0;
            else {
                idx_t half = m >> 1;             /* m / 2 */
                idx_t t = _floor_isqrt_u64(half);
                include = (t * t != half);       /* exclude twice a square */
            }
        }

        if (include) {                         /* valid member of the sequence */
            count++;
            if (count == n)
                return (seq_t)m;                /* found the nth element */
        }
        ++m;                                   /* next candidate */
    }
}
