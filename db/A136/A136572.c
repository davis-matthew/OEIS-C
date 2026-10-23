/*
 * A136572  Triangle read by rows: row n consists of n zeros followed by n!.
 * https://oeis.org/A136572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r! if n = r(r+3)/2 for some integer r≥0; otherwise a(n)=0
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 664, big any n
 * Helpers:   factorial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute 8*n + 9 in seq_t to avoid overflow on large n. */
    seq_t t = ((seq_t)n) * 8 + 9;
    /* Integer square root of t. */
    seq_t s = floor_sqrt(t);
    /* Check perfect square condition. */
    if (s * s != t)
        return 0;
    /* s must be at least 3; otherwise r would be negative. */
    if (s < 3)
        return 0;
    /* Compute r = (s - 3)/2.  s-3 must be even. */
    seq_t diff = s - 3;
    if ((diff & 1) != 0)      /* odd -> no integer r */
        return 0;
    seq_t r = diff >> 1;      /* r ≥ 0 */
    /* For safety, verify that r(r+3)/2 equals n (prevents overflow errors). */
    idx_t check = (idx_t)r * ((idx_t)r + 3) / 2;
    if (check != n)
        return 0;
    /* Return factorial of r. */
    return factorial((idx_t)r);
}
