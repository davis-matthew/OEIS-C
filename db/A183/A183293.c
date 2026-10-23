/*
 * A183293  Complement of A056000.
 * https://oeis.org/A183293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m - 4 if 2*n <= (m-3)*(m+4); * Otherwise a(n) = n + m
 *            - 3, * where m = floor_sqrt(8*n + 49) / 2. * This implements
 *            OEIS A183293 (Complement of A056000)
 * Range:     u8 n <= 1, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 8*n + 49 – use 128‑bit arithmetic to avoid overflow
     * for very large n when seq_t is 64‑bit. */
    unsigned __int128 temp = (unsigned __int128)8 * n + (unsigned __int128)49;
    seq_t eight_n = (seq_t)temp;

    /* m = floor_sqrt(8*n + 49) / 2 */
    seq_t m = floor_sqrt(eight_n) / 2;

    /* Condition: 2*n <= (m-3)*(m+4) */
    seq_t lhs = (seq_t)2 * (seq_t)n;
    seq_t rhs = (m - (seq_t)3) * (m + (seq_t)4);

    if (lhs <= rhs)
        return (seq_t)n + m - (seq_t)4;
    else
        return (seq_t)n + m - (seq_t)3;
}
