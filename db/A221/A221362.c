/*
 * A221362  Number of distinct groups of order n that are the torsion subgroup
 *          of an elliptic curve over the rationals Q.
 * https://oeis.org/A221362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = number of Mazur torsion groups of order n
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    /* Cyclic groups Z/NZ, for N = 1,2,…,10,12. */
    const idx_t cyclicN[] = {1,2,3,4,5,6,7,8,9,10,12};
    const int c_count = 11;

    for (int i = 0; i < c_count; ++i) {
        if (n == cyclicN[i]) ++cnt;
    }

    /* Product groups Z/2Z × Z/2N, N = 1…4  (order = 4·N). */
    for (idx_t N = 1; N <= 4; ++N) {
        if (n == 4 * N) ++cnt;
    }

    return cnt;
}
