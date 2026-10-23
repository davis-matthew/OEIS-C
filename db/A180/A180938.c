/*
 * A180938  Smallest k such that k*n has an even number of 1's in its base-2
 *          expansion.
 * https://oeis.org/A180938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 9925, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for a given n, find the minimal odd k such that
   the binary expansion of k*n has an even number of 1's. */
#include <stdint.h>

static unsigned popcount_seq_t(seq_t x)
{
    /* Count set bits of an arbitrary unsigned integer type. */
    unsigned count = 0;
    while (x)
    {
        count += (unsigned)(x & 1);
        x >>= 1;
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* ensure arithmetic in seq_t */
    for (seq_t k = 1; ; k += 2)  /* k must always be odd */
    {
        seq_t product = k * N;
        if ((popcount_seq_t(product) & 1) == 0)
            return k;           /* even popcount found */
    }
}
