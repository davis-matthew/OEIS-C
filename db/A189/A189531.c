/*
 * A189531  n+[nr/s]+[nt/s]; r=1, s=2^(1/3), t=2^(2/3).
 * https://oeis.org/A189531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n⋅2^(1/3)⌋ + ⌊n⋅2^(-1/3)⌋ The last term is computed
 *            via ⌊⌊n⋅2^(2/3)⌋/2⌋, which equals ⌊n⋅2^(-1/3)⌋. Integer
 *            arithmetic is used; no floating point operations
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   _ucube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_ucube_root_floor.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for the sequence, but return 0 safely */
        return 0;

    seq_t nn = (seq_t)n;           /* n as seq_t */
    unsigned __int128 n128 = (unsigned __int128)n;

    unsigned __int128 n3 = n128 * n128 * n128;   /* n^3 */
    unsigned __int128 x1 = n3 * 2;                /* 2 * n^3  */
    unsigned __int128 x2 = n3 * 4;                /* 4 * n^3  */

    unsigned __int128 floor_n_c  = _ucube_root_floor(x1); // ⌊n⋅2^(1/3)⌋
    unsigned __int128 floor_n_c2 = _ucube_root_floor(x2); // ⌊n⋅2^(2/3)⌋

    seq_t part1 = (seq_t)floor_n_c;
    seq_t part2 = ((seq_t)floor_n_c2) / 2;          /* ⌊⌊n⋅2^(2/3)⌋/2⌋ = ⌊n⋅2^(-1/3)⌋ */

    return nn + part1 + part2;
}
