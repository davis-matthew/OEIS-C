/*
 * A205121  Least k such that n divides a difference between distinct numbers
 *          j*(2^(j-1)), when ordered as in A205120.
 * https://oeis.org/A205121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   least k such that n divides a difference between distinct
 *            numbers j*(2^(j-1)), when ordered as in A205120
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but safe guard */

    /* Allocate an array to remember the first occurrence of each residue. */
    size_t N = (size_t)n;
    idx_t *first = (idx_t *)malloc(N * sizeof(idx_t));
    if (!first) return 0;           /* allocation failed */
    memset(first, 0, N * sizeof(idx_t));

    /* 2^(j-1) mod n for the current j.  Start with j = 1. */
    seq_t pow2 = 1 % n;

    for (idx_t j = 1;; ++j) {
        /* f(j) = j * 2^(j-1) mod n */
        seq_t fval = ((seq_t)j * pow2) % n;
        idx_t rem = (idx_t)fval;

        if (j == 1) {
            first[rem] = j;
        } else {
            idx_t i = first[rem];
            if (i != 0) {
                /* Found first pair (i, j) with equal residue.  The
                   total number of pairs processed up to (i, j) is
                   ((j-1)*(j-2))/2 + i. */
                seq_t k = ((seq_t)(j - 1) * (j - 2)) / 2 + i;
                free(first);
                return k;
            }
            first[rem] = j;
        }

        /* Prepare pow2 for the next j (increase exponent by 1). */
        pow2 = ((seq_t)(((__uint128_t)pow2 * 2) % n));
    }
}
