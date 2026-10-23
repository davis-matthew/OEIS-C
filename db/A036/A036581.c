/*
 * A036581  Ternary Thue-Morse sequence: closed under a->abc, b->ac, c->b.
 * https://oeis.org/A036581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A010060(n+1) - A010060(n) - 1 mod 3 (where A010060(n) =
 *            parity of popcount(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* popcount of n and n+1, mod 2 */
    int t_n   = __builtin_popcountll(n) & 1;
    int t_np1 = __builtin_popcountll(n + 1) & 1;

    int d = t_np1 - t_n - 1;     /* difference, may be negative */
    if (d < 0) d += 3;           /* normalize to 0..2 modulo 3 */

    return (seq_t)d;
}
