/*
 * A047541  Numbers that are congruent to {1, 2, 4, 7} mod 8.
 * https://oeis.org/A047541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + [1,2,4,7][(n-1)%4] (n ≥ 1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* residues that are allowed modulo 8 */
    static const seq_t r[4] = {1, 2, 4, 7};

    /* compute group number and position within the group */
    idx_t g   = (n - 1) / 4;                 /* floor((n-1)/4)          */
    size_t pos = (size_t)((n - 1) % 4);      /* (n-1) mod 4 (0…3)       */

    /* result = 8 * g + r[pos] */
    return (seq_t)g * (seq_t)8 + r[pos];
}
