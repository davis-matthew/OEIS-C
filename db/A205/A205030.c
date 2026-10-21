/*
 * A205030  The number s(j)=j*(j+1) such that n divides s(k)-s(j)>0, where k
 *          is the least positive integer for which such a j exists.
 * https://oeis.org/A205030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the value s(j) = j*(j+1) where the smallest k>0 * such
 *            that s(k) ≡ s(j) (mod n) occurs is found; j is the first * index
 *            with that residue. Thus a(n) is the value of s(j) for the *
 *            earliest repeat of residues modulo n in the sequence
 *            s(m)=m*(m+1)
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* allocate array for first occurrences of residues modulo n */
    size_t sz = (size_t)n + 1;               /* indices 0..n-1 */
    idx_t *first = (idx_t*)malloc(sz * sizeof(idx_t));
    if (!first) return (seq_t)0;             /* unlikely */ 
    memset(first, 0, sz * sizeof(idx_t));    /* 0 means “not seen” */

    for (idx_t k = 1; ; ++k) {
        /* s(k) = k*(k+1) as seq_t */
        seq_t sk = (seq_t)k * ((seq_t)k + 1);
        /* residue modulo n */
        idx_t r = (idx_t)(sk % (seq_t)n);

        if (first[r] == 0) {
            /* first time this residue occurs */
            first[r] = k;
        } else {
            /* found repetition: j = first[r] */
            idx_t j = first[r];
            seq_t sj = (seq_t)j * ((seq_t)j + 1);
            free(first);
            return sj;
        }
    }
}
