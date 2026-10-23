/*
 * A190311  Number of nonzero digits when writing n in base where place values
 *          are positive cubes, cf. A000433.
 * https://oeis.org/A190311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonzero digits in the “cubic number system”
 *            representation of n. The cubic number system uses place values
 *            k³ (k ≥ 1)
 * Range:     u8 n <= 1132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external declaration provided by the build system:
   idx_t int_cube_root(idx_t x);  // floor of cube root of x */
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;
    seq_t remaining = (seq_t)n;
    /* choose largest k such that k³ ≤ remaining */
    idx_t k = int_cube_root((idx_t)remaining);

    while (remaining > 0 && k > 0)
    {
        /* compute k³ in seq_t arithmetic */
        seq_t cube = (seq_t)k * (seq_t)k * (seq_t)k;
        seq_t d = remaining / cube;
        if (d > 0)
        {
            count++;
            remaining -= d * cube;
        }
        k--;
    }
    return count;
}
