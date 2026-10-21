/*
 * A205403  a(n) is the index j < k such that n divides s(k)-s(j) for some j,
 *          where s(j) = floor((j+1)^2/2)/2, and k is the least index for
 *          which such a j exists.
 * https://oeis.org/A205403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) is the index j < k such that n divides s(k) - s(j) where
 *            * s(j) = floor((j+1)^2 / 4). For each n the first repeated *
 *            residue of s(j) modulo n gives the minimal k and the earlier j *
 *            is returned as a(n)
 * Range:     u8 n <= 38, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                 /* offset 1; not used */
    }

    /* Allocate array of first occurrences of residues modulo n.
       Index 0 indicates “not seen yet”; j starts from 1. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) {
        abort();                   /* memory allocation failure */
    }

    for (idx_t k = 1; ; ++k) {
        /* s(k) = floor((k+1)^2 / 4) */
        seq_t kp1 = (seq_t)(k + 1);
        seq_t sq   = kp1 * kp1;    /* (k+1)^2 as seq_t */
        seq_t s    = sq / 4;       /* integer division by 4 */
        idx_t r    = (idx_t)(s % (seq_t)n);  /* residue modulo n */

        if (first[r] == 0) {
            /* first time we see this remainder */
            first[r] = k;
        } else {
            /* repetition found: earlier j = first[r] */
            seq_t result = (seq_t)first[r];
            free(first);
            return result;
        }
    }

    /* Should never be reached */
    free(first);
    return 0;
}
