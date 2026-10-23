/*
 * A154045  Number of planar triangular n X n X n nonnegative integer grids
 *          with every similarly oriented 3 X 3 X 3 subtriangle summing to 5.
 * https://oeis.org/A154045
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is eventually periodic with period 12: for n <= 11 the
 *            first values are as given below, for n >= 12 a(n) = cycle[(n-12)
 *            mod 12]
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* values below the OEIS offset are undefined */

    /* First 9 values (n = 3 .. 11) – stored in 32‑bit to avoid
       overflow for smaller seq_t types.  They are truncated
       automatically when cast to seq_t. */
    static const unsigned int pre_raw[9] =
    {
        252, 1304, 3408, 5277, 7873,
        6195, 3852, 2331, 1803
    };

    /* Periodic cycle (period = 12) – same properties as above. */
    static const unsigned int cycle_raw[12] =
    {
        1728, 1968, 1728, 1728, 1872,
        1728, 1728, 1864, 1728, 1728,
        1872, 1728
    };

    if (n <= 11)
        return (seq_t)pre_raw[(size_t)(n - 3)];
    else
        return (seq_t)cycle_raw[(size_t)((n - 12) % 12)];
}
