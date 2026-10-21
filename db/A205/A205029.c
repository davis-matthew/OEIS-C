/*
 * A205029  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=j*(j+1).
 * https://oeis.org/A205029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least s(k) where s(k)=k*(k+1) is congruent modulo n to
 *            some s(j) with j<k. This is the first repeated residue of the
 *            sequence k*(k+1) modulo n, and the minimal repeating value is
 *            returned
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, but safe */
    /* Allocate a temporary table for residues modulo n */
    bool *seen = (bool *)malloc((size_t)n * sizeof(bool));
    if (!seen) return (seq_t)0;           /* memory failure fallback */
    for (size_t i = 0; i < n; ++i) seen[i] = false;

    /* Scan k = 1,2,... until a residue repeats */
    for (idx_t k = 1;; ++k) {
        seq_t sk = (seq_t)k * ((seq_t)k + 1);
        idx_t residue = (idx_t)(sk % n);
        if (seen[residue]) {             /* first repetition found */
            free(seen);
            return sk;
        }
        seen[residue] = true;
    }

    /* never reached, but satisfy compiler */
    free(seen);
    return (seq_t)0;
}
