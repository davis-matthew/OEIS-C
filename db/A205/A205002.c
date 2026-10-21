/*
 * A205002  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=j(j+1)/2.
 * https://oeis.org/A205002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that there exists j<k with
 * Range:     u8 n <= 255, u16 n <= 8080, u32 n <= 8080, u64 n <= 8080,
 *            u128 n <= 8080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              n divides T_k - T_j, where T_i = i(i+1)/2.  */
/*  This is equivalent to the first repetition of triangular numbers */
/*  modulo n; we return the index of the second occurrence of a residue. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1, but safe */

    /* Allocate array to store first index seen for each residue modulo n */
    size_t sz = (size_t)n + 1;        /* +1 for 0 residue */
    idx_t *first = (idx_t *)calloc(sz, sizeof(idx_t));
    if (first == NULL)
        return 0;                     /* out‑of‑memory guard */

    for (idx_t k = 1; ; ++k) {
        /* Compute T_k mod n safely using idx_t arithmetic */
        idx_t t = ((k * (k + 1)) / 2) % n;

        if (first[t] == 0) {
            first[t] = k;             /* first time this residue appears */
        } else {
            /* second occurrence: k is the desired a(n) */
            free(first);
            return (seq_t)k;
        }
    }
}
