/*
 * A083380  a(n) is the number of cubes with at most n digits and first digit
 *          1.
 * https://oeis.org/A083380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Σ_{d=1..n} (⌊∛(2·10^{d-1} – 1)⌋ – ⌊∛(10^{d-1} – 1)⌋)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   floor_cube_root_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root_128.h"
/*  The following implementation keeps all intermediate arithmetic in 128‑bit
    unsigned integers (a type available in C11/C++17).  This avoids overflow
    for all indices where the result still fits into the regular 64‑bit
    integer type.  For an arbitrary‑precision seq_t the same code works,
    because the conversion from a 128‑bit value to seq_t is well defined. */

/* ------------------------------- Main function -------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1)         /* undefined below the OEIS offset */
        return 0;

    seq_t   total = 0;           /* running sum */

    unsigned __int128 pow10 = 1; /* represents 10^{d-1} as 128‑bit int   */

    for (idx_t d = 1; d <= n; ++d) {
        /* lower bound: 10^{d-1} – 1 */
        unsigned __int128 lowB = pow10 - 1;
        seq_t lowRoot = floor_cube_root_128(lowB);

        /* upper bound: 2·10^{d-1} – 1 */
        unsigned __int128 highB = pow10 * 2 - 1;
        seq_t highRoot = floor_cube_root_128(highB);

        total += highRoot - lowRoot;

        /* prepare next power of 10 */
        pow10 *= 10;
    }
    return total;
}
