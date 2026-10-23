/*
 * A161415  First differences of A160414.
 * https://oeis.org/A161415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n = 1, a(1) = 1. For n > 1, let k = popcount(n-1) be the
 *            number of 1‑bits of n-1. Then a(n) = 4 * 3^k but if n is a power
 *            of two subtract 2*n: a(n) = 4 * 3^k - 2*n. This implements the
 *            definition of A161415
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count 1-bits in a 64‑bit integer. */
static int popcount_idx(idx_t x)
{
    int c = 0;
    while (x)
    {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute 3^{popcount(n-1)}. */
    idx_t m = n - 1;
    int pc = popcount_idx(m);
    seq_t val = (seq_t)1;
    for (int i = 0; i < pc; ++i)
        val *= (seq_t)3;

    /* Multiply by 4. */
    val *= (seq_t)4;

    /* If n is a power of two, subtract 2*n. */
    if ((n & (n - 1)) == 0)
        val -= (seq_t)2 * (seq_t)n;

    return val;
}
