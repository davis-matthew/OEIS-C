/*
 * A076826  a(n) = 2*(Sum_{k=0..n} A010060(k)) - n, where A010060 is a
 *          Thue-Morse sequence.
 * https://oeis.org/A076826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(sum_{k=0}^{n} A010060(k)) - n Equivalent recurrence
 *            used: a(2k) = 2*A010060(k) (0 if popcount(k) even, 1 if odd)
 *            a(2k+1) = 1 where A010060(k) is parity of popcount(k)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 122 terms through n = 7957 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint8_t parity(idx_t x)
{
    /* Xor folding to get parity; final bit is 1 if popcount is odd */
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}

seq_t a(idx_t n)
{
    if (n & 1)               /* n is odd → a(2k+1) = 1 */
        return (seq_t)1;
    else                     /* n is even  → a(2k) = 2 * parity(k) */
    {
        idx_t k = n >> 1;
        return (seq_t)(2 * (seq_t)parity(k));
    }
}
