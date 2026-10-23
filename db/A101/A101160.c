/*
 * A101160  a(n) is the smallest integer j for which n+(n+1)+...+(n+j) is a
 *          square.
 * https://oeis.org/A101160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer j ≥ 0 such that n + (n+1) + ... +
 *            (n+j) is a perfect square
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: the is_square helper is provided by the build system. */
#include <stdbool.h>
#include "../utils/is_square.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for calculations */
    seq_t nseq = (seq_t)n;
    /* maximum possible j is 2*(n-1) */
    idx_t jmax = 2 * n - 2;
    for (idx_t j = 0; j <= jmax; ++j) {
        /* sum of arithmetic progression from n to n+j: (2n + j)*(j+1)/2 */
        seq_t sum = ((seq_t)(2 * nseq) + j) * (j + 1) / 2;
        if (is_square(sum))
            return (seq_t)j;
    }
    /* Theoretical guarantee guarantees a solution ≤ 2*(n-1). */
    return 0;
}
