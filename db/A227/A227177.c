/*
 * A227177  n occurs n^2 - n + 1 times.
 * https://oeis.org/A227177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m+1 if 3n > m*(m^2 + 2) else m, * where m = ⌊∛(3n)⌋. This
 *            follows the OEIS definition: * a(k + (j^3-j^2+5*j)/3) = j for
 *            all j≥0, k=0…(j^2-j)
 * Range:     u8 n <= 9951, u16 n <= 9951, u32 n <= 9951, u64 n <= 9951,
 *            u128 n <= 9951, big any n
 * Helpers:   cube_root_floor128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer cube root of a 128‑bit value via binary search. */
#include <stdint.h>
#include "../utils/cube_root_floor128.h"

seq_t a(idx_t n)
{
    /* Compute 3n (may overflow uint64_t) using 128‑bit arithmetic. */
    unsigned __int128 three_n = (unsigned __int128)n * 3;
    idx_t m = cube_root_floor128(three_n);

    /* Check the inequality: 3n > m*(m^2 + 2). */
    unsigned __int128 rhs = (unsigned __int128)m *
                            ((unsigned __int128)m * m + 2);

    if (three_n > rhs)
        return (seq_t)(m + 1);
    else
        return (seq_t)m;
}
