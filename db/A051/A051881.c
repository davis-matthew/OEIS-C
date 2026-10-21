/*
 * A051881  Number of subgroups of order n in special orthogonal group SO(3).
 * https://oeis.org/A051881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + [n is even and n>2] + [n == 12 || n == 24 || n == 60]
 * Range:     u8 n <= 9797, u16 n <= 9797, u32 n <= 9797, u64 n <= 9797,
 *            u128 n <= 9797, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* trivial subgroup */
    seq_t res = 1;

    /* add the dihedral subgroup when the index is even and > 2 */
    if ((n & 1) == 0) {
        if (n > 2)
            res += 1;
    }

    /* exceptional platonic groups */
    if (n == 12 || n == 24 || n == 60)
        res += 1;

    return res;
}
