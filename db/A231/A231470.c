/*
 * A231470  Largest integer less than 10, coprime to n.
 * https://oeis.org/A231470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest integer < 10 that is coprime to n
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    for (int c = 9; c >= 1; --c)
    {
        if (gcd(N, (seq_t)c) == (seq_t)1)
            return (seq_t)c;
    }
    return (seq_t)1; /* fallback, should never reach */
}
