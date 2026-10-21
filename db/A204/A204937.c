/*
 * A204937  (k(n)!-j(n)!)/n, where (k!,j!) is the least pair of distinct
 *          factorials for which n divides k!-j!.
 * https://oeis.org/A204937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k(n)! - j(n)!)/n, where (k(n), j(n)) is the first pair
 *            of distinct factorials with equal residues modulo n (k > j and
 *            k! ≠ j!). The algorithm finds the smallest k for which the
 *            factorial residue repeats and then returns the corresponding
 *            quotient
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 36, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for n=0, return 0 */
    /* Allocate array to remember the first index where each residue appeared. */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return 0;                /* allocation failure handled trivially */
    for (size_t i = 0; i < n; ++i)
        first[i] = (idx_t)-1;            /* mark as unseen */

    seq_t fac = 1;                       /* 0! */
    /* store residue of 0! */
    idx_t r = (idx_t)(fac % n);
    first[r] = 0;

    /* iterate over k = 1, 2, ... */
    for (idx_t k = 1; ; ++k) {
        fac = fac * k;                   /* k! */
        r   = (idx_t)(fac % n);          /* residue k! mod n */

        if (first[r] != (idx_t)-1) {
            idx_t j = first[r];
            /* recompute j! exactly */
            seq_t fac_j = 1;
            for (idx_t t = 1; t <= j; ++t)
                fac_j = fac_j * t;

            seq_t diff = fac - fac_j;
            if (diff == 0) {
                /* skip the trivial equal factorial pair (e.g., 0! and 1!) */
                continue;
            }
            seq_t ans = diff / (seq_t)n; /* division truncates as required */
            free(first);
            return ans;
        } else {
            /* first occurrence of this residue */
            first[r] = k;
        }
    }
}
