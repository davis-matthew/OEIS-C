/*
 * A205142  s(k)-s(j), where (s(k),s(j)) is the least such pair for which n
 *          divides their difference, and s(j)=j(3j-1)/2, the j-th pentagonal
 *          number.
 * https://oeis.org/A205142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205142: a(n) = s(k)-s(j) where (j,k) is the first pair such that
   s(k) ≡ s(j) (mod n) with s(t)=t(3t-1)/2 (t‑th pentagonal number).  */

/*  The algorithm scans pentagonal numbers modulo n until the first
    residue repeats.  The j of that first repeated residue is minimal
    (earliest appearance), and k is the current index.  The difference
    s(k)-s(j) is then returned.  This is O(n) time and O(n) memory
    per call.  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n_idx)
{
    idx_t n = (idx_t)n_idx;       /* n ≥ 1 */

    /* Allocate array for first occurrence of each residue 0 … n-1.  */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return (seq_t)0;  /* allocation failure – unlikely for small n */
    memset(first, 0, (size_t)n * sizeof(idx_t));

    for (idx_t j = 1; ; ++j) {
        /* s(j) = j(3j-1)/2 */
        seq_t sj = (seq_t)j * ((seq_t)j * 3 - 1) / 2;

        idx_t r = (idx_t)(sj % n);
        if (first[r] == 0) {
            first[r] = j;   /* remember first occurrence */
        } else {
            idx_t j0 = first[r];
            seq_t sj0 = (seq_t)j0 * ((seq_t)j0 * 3 - 1) / 2;
            seq_t diff = sj - sj0;
            free(first);
            return diff;
        }
    }
}
