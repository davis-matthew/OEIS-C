/*
 * A205006  a(n) = s(k)-s(j), where (s(k),s(j)) is the least pair of distinct
 *          triangular numbers for which n divides their difference.
 * https://oeis.org/A205006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal difference of two distinct triangular numbers
 *            s(k)-s(j) such that n | (s(k)-s(j)), with (k>j) the first pair
 *            that satisfies the divisibility condition
 * Range:     u8 n <= 46, u16 n <= 1578, u32 n <= 8683, u64 n <= 8683,
 *            u128 n <= 8683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out‑of‑range, should not occur */

    /* array mapping residue modulo n to the first index that produced it */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return 0;           /* allocation failure */

    for (idx_t i = 0; i < n; ++i) first[i] = 0;

    /* work in seq_t for arithmetic */
    const seq_t nn = (seq_t)n;      /* n as seq_t */
    seq_t tk = 1;                   /* T1 */
    idx_t k = 1;                    /* current index */

    /* record residue of T1 */
    idx_t r = (idx_t)(tk % nn);
    first[r] = k;

    /* generate triangular numbers until a residue repeats */
    for (++k; ; ++k) {
        tk += (seq_t)k;             /* T_k = T_{k-1} + k */
        r = (idx_t)(tk % nn);
        idx_t j = first[r];
        if (j == 0) {
            first[r] = k;           /* first occurrence of this residue */
        } else {
            seq_t tj = (seq_t)j * ((seq_t)j + 1) / 2;
            seq_t diff = tk - tj;
            free(first);
            return diff;
        }
    }
}
