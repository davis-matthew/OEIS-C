/*
 * A061728  Start with (a, b) = (2, 4). The next pair (a', b') is ((b + 1) mod
 *          10, (a + 1) mod 10) where (a, b) is the previous pair.
 * https://oeis.org/A061728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (2*⌊n/4⌋ + ⌊3*(n mod 4)/2⌋ + 1) mod 10
 * Range:     u8 n <= 524, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The operation uses only integer arithmetic and runs in O(1). */
    if (n == 0)   /* out of range; return a default value */
        return (seq_t)0;

    idx_t q   = n / 4;          /* floor(n/4)   */
    idx_t r   = n % 4;          /* n mod 4      */
    seq_t t1  = (seq_t)(2 * q);                       /* 2*floor(n/4)   */
    seq_t t2  = (seq_t)(3 * r / 2);                    /* floor(3*(n mod4)/2) */
    seq_t res = (t1 + t2 + 1) % 10;                    /* final formula   */

    return res;
}
