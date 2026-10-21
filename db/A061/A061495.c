/*
 * A061495  a(n) = lcm(3n+1, 3n+2, 3n+3).
 * https://oeis.org/A061495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(3n+1, 3n+2, 3n+3) For n even all three factors are
 *            pairwise coprime, hence a(n) = (3n+1)(3n+2)(3n+3). For n odd
 *            exactly two factors share a factor 2, so a(n) =
 *            [(3n+1)/2]·(3n+2)·(3n+3)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 541, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    seq_t a = sn * 3 + 1;         /* 3n+1 */
    seq_t b = sn * 3 + 2;         /* 3n+2 */
    seq_t c = sn * 3 + 3;         /* 3n+3 */

    if (n & 1)                    /* n odd → divide one even factor by 2 */
        a >>= 1;                  /* (3n+1) is even when n is odd        */

    return a * b * c;
}
