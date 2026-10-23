/*
 * A025627  Numbers of form 6^i*8^j, with i, j >= 0.
 * https://oeis.org/A025627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 78, u64 n <= 287,
 *            u128 n <= 1103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate numbers of the form 6^i * 8^j (i,j≥0) sorted ascending.
   Uses a two‑pointer merge similar to the "ugly numbers" algorithm. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        seq_t one = 1;
        return one;
    }

    /* Allocate array for the first n terms (indices 0 .. n-1). */
    seq_t *seq = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (!seq) return (seq_t)0;   /* unlikely in test environment */

    seq[0] = (seq_t)1;
    idx_t p6 = 0;   /* next index to multiply by 6 */
    idx_t p8 = 0;   /* next index to multiply by 8 */

    for (idx_t k = 1; k < n; ++k) {
        seq_t cand6 = seq[p6] * (seq_t)6;
        seq_t cand8 = seq[p8] * (seq_t)8;
        seq_t next;

        if (cand6 < cand8) {
            next = cand6;
            ++p6;
        } else if (cand8 < cand6) {
            next = cand8;
            ++p8;
        } else {           /* equal – avoid duplicate */
            next = cand6;
            ++p6;
            ++p8;
        }
        seq[k] = next;
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
