/*
 * A204995  The index j<k such that n divides k^2-j^2, where k is the least
 *          index (A204905) for which such j exists.
 * https://oeis.org/A204995
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

/* A204995: For a given n find the smallest index j < k such that
   n divides k² − j², where k is the least index for which such a j exists.
   The computation proceeds by generating squares modulo n.  For each
   residue r the first index where r appears is stored; the first time a
   residue repeats, its initial index is the desired j.  The Euclidean
   algorithm is not required, simply a linear scan.  The algorithm
   runs in O(n) time and uses O(n) auxiliary space. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* not defined for n=0; return 0 safely */
        return 0;

    /* array to record first occurrence of each residue modulo n */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (first == NULL)   /* allocation failed */
        return 0;

    seq_t answer = 0;
    for (idx_t k = 1; ; ++k) {
        /* compute k^2 (mod n) */
        seq_t sq = (seq_t)k * (seq_t)k;
        seq_t mod = sq % (seq_t)n;
        idx_t r = (idx_t)mod;

        if (first[r] == 0) {
            /* first time this residue appears */
            first[r] = k;
        } else {
            /* repeat found: j is the stored index */
            answer = first[r];
            break;
        }
    }

    free(first);
    return answer;
}
