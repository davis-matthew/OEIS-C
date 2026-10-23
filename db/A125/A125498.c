/*
 * A125498  Odious cubes.
 * https://oeis.org/A125498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 821, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate through natural numbers, test the cube of each for
 * an odd number of 1's in its binary representation, and return the
 * nth such cube (offset 1). */
#include <stdbool.h>

/* Return true if the number of 1 bits in x is odd. */
static bool popcount_parity(seq_t x)
{
    bool odd = false;
    while (x)
    {
        if ((x & (seq_t)1) != 0) odd = !odd;
        x >>= 1;
    }
    return odd;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined for n=0 */

    idx_t k = 1;                    /* candidate base number */
    idx_t count = 0;                /* number of odious cubes found */

    while (1)
    {
        seq_t kk = (seq_t)k;
        seq_t cube = kk * kk * kk;  /* compute k^3 in seq_t */

        if (popcount_parity(cube))
        {
            ++count;
            if (count == n) return cube;
        }
        ++k;
    }
}
