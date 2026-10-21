/*
 * A205031  s(k)-s(j), where (s(k),s(j)) is the least pair of oblong numbers
 *          for which n divides their difference.
 * https://oeis.org/A205031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205031:
   a(n) = minimal positive difference of two oblong numbers
   (s(k)=k*(k+1)) for which n divides the difference.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array to record the first k that produced each residue modulo n. */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first) return 0;                       /* out‑of‑memory guard */

    for (idx_t k = 1; ; ++k) {
        seq_t sk = (seq_t)k * (seq_t)(k + 1);   /* oblong number s(k) */
        idx_t residue = (idx_t)(sk % n);        /* residue modulo n */

        if (!first[residue]) {
            /* First time this residue appears. */
            first[residue] = k;
        } else {
            /* Match found: compute the minimal difference for the smallest k. */
            idx_t j = first[residue];
            seq_t sj = (seq_t)j * (seq_t)(j + 1);
            seq_t diff = sk - sj;
            free(first);
            return diff;
        }
    }
}
