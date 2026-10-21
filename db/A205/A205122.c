/*
 * A205122  Least k such that n divides s(k)-s(j) for some j satisfying
 *          1<=j<k, where s(j)=j*(2^(j-1)).
 * https://oeis.org/A205122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least k such that n divides s(k) - s(j) for some 1 <= j < k,
   where s(j) = j * 2^(j-1).  We compute the residues of s(k) modulo n
   and return the first k for which a residue repeats. */

#include <stdlib.h>
#include <string.h>

/* seq_t and idx_t are defined in the build. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but just return 0 to avoid UB */
        return (seq_t)0;

    /* Allocate a seen array of n bytes, initialized to 0. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, 1);
    if (!seen) /* allocation failure: return 0 as safe fallback */
        return (seq_t)0;

    seq_t pow2 = (seq_t)1 % (seq_t)n; /* 2^(k-1) mod n, starts at k=1 */
    for (idx_t k = 1;; ++k) {
        /* Compute s(k) modulo n: r = k * 2^(k-1) mod n */
        seq_t r = ((seq_t)k * pow2) % (seq_t)n;
        idx_t idx = (idx_t)r;
        if (seen[idx]) {          /* residue seen before → answer */
            free(seen);
            return (seq_t)k;
        }
        seen[idx] = 1;            /* mark residue as seen */

        /* Prepare pow2 for next k: 2^k mod n */
        pow2 = (pow2 * (seq_t)2) % (seq_t)n;
    }
}
