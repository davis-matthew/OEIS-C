/*
 * A138033  a(n) = max_{ 1 <= i <= n-1 } min{ wt(i), wt(n-i) }, where wt() =
 *          A000120() is the binary weight function; a(1) = 0 by convention.
 * https://oeis.org/A138033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{1 <= i <= n-1} min{ wt(i), wt(n-i) } where wt() is
 *            the binary weight (popcount). a(1)=0 by convention
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the binary weight (popcount) of a 64-bit unsigned integer. */
static inline seq_t popcnt(idx_t x)
{
    /* __builtin_popcountll is available in GCC/Clang and returns unsigned int */
    return (seq_t)__builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t best = 0;
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t w1 = popcnt(i);
        seq_t w2 = popcnt(n - i);
        seq_t m  = (w1 < w2) ? w1 : w2;
        if (m > best)
            best = m;
    }
    return best;
}
