/*
 * A205140  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=j(3j-1)/2, the j-th pentagonal number.
 * https://oeis.org/A205140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least pentagonal number s(k) with k>j≥1 such that * n
 *            divides s(k)-s(j), where s(j)=j(3j-1)/2 (the j‑th pentagonal
 *            number). * The algorithm searches for the first repeated residue
 *            modulo n among * the pentagonal numbers (starting at j=1). The
 *            corresponding s(k) is * returned. This works in...
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static cache for residue seen array */
static bool *seen = NULL;
static size_t seen_cap = 0;

seq_t a(idx_t n)
{
    /* n ≥ 1, see OEIS offset */
    size_t nn = (size_t)n;             /* number of residues 0 … n‑1 */
    if (nn > seen_cap) {
        free(seen);
        seen = (bool *)malloc(nn * sizeof(bool));
        seen_cap = nn;
    }
    /* initialise: no residues seen yet */
    memset(seen, 0, nn * sizeof(bool));

    /* iterate pentagonal numbers j = 1,2,… until a repeat occurs */
    for (idx_t j = 1; ; ++j) {
        /* compute s(j) = j(3j-1)/2 as seq_t */
        seq_t sj = (seq_t)j;
        seq_t s   = sj * ((seq_t)3 * sj - 1) / 2;

        /* residue of s modulo n (using seq_t arithmetic) */
        idx_t r = (idx_t)(s % (seq_t)n);
        if (seen[r]) {
            return s;          /* first repeated residue found */
        }
        seen[r] = true;
    }
}
