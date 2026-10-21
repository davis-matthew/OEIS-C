/*
 * A205004  Least k(k+1)/2 such that n divides k(k+1)/2-j(j+1)/2 for some j<k.
 * https://oeis.org/A205004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least triangular number T_k = k(k+1)/2 such that,
 *            for some 1 ≤ j < k, the difference T_k − T_j is divisible by n.
 *            The algorithm searches for the first repeated residue of the
 *            triangular numbers modulo n
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation runs in O(n) time and uses O(n) memory. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is always ≥ 1 */
    size_t N = (size_t)n;          /* array size fits into size_t for the intended range */
    uint8_t *seen = (uint8_t *)malloc(N);
    if (!seen)                      /* allocation failed – return 0 as sentinel */
        return (seq_t)0;
    for (size_t i = 0; i < N; ++i)   /* initialise all residues as unseen */
        seen[i] = 0;

    size_t rem = 0;                 /* T_0 mod n */
    size_t k   = 0;                 /* current index (starts from 0) */

    while (1) {
        ++k;                        /* move to next k (≥ 1) */
        rem = (rem + k) % N;        /* T_k mod n via cumulative sum of k */

        if (seen[rem]) {            /* residue already seen – repeat found */
            seq_t K = (seq_t)k;
            seq_t ans = K * (K + 1) / 2;
            free(seen);
            return ans;
        }
        seen[rem] = 1;               /* mark residue as seen for future k */
    }
}
