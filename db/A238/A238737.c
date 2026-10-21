/*
 * A238737  a(n) = 2*n+2 - A224911(n).
 * https://oeis.org/A238737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 2 - (largest prime ≤ 2*n + 2)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = 2*n + 2 - A224911(n)   */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)2 + (seq_t)2;   /* m = 2*n + 2 */
    seq_t p = m;                               /* start from m */
    while (!is_prime_big(p))                   /* find largest prime ≤ m */
    {
        p -= (seq_t)1;
    }
    return m - p;                              /* a(n) = 2*n+2 - p */
}
