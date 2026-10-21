/*
 * A204936  k!-j!, where (k!,j!) is the least pair of distinct factorials for
 *          which n divides k!-j!.
 * https://oeis.org/A204936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive difference k! – j! such that (k!, j!)
 *            is the least pair of distinct factorials with n | (k! – j!)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by examining factorials modulo n.  For each
   residue r, the first time it appears we remember the corresponding
   factorial value.  Whenever the same residue appears again we have
   found a pair whose difference is a multiple of n.  The smallest
   positive such difference is the required term.  The algorithm runs
   in O(n) time and uses at most O(n) storage.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined for n = 0, but return 0 to keep function defined. */
        return (seq_t)0;
    }

    /* Arrays to keep track of whether a residue has been seen and the
       factorial value at its first occurrence.  Size n is safe because
       residues are 0..n-1.  */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return (seq_t)0; /* allocation failed */
    seq_t *first_val = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!first_val) {
        free(seen);
        return (seq_t)0;
    }

    seq_t best = (seq_t)0;   /* 0 indicates no positive diff found yet */
    bool found = false;

    seq_t f = (seq_t)1;          /* factorial(0) = 1 */
    seq_t mod = (seq_t)n;        /* modulus for residue calculation */

    /* Iterate over m = 0 .. n+1 inclusive.  After computing the
       residue for factorial(m), we update f to factorial(m+1)
       for the next iteration. */
    for (idx_t m = 0; m <= n + 1; ++m) {
        /* Compute residue r = factorial(m) mod n.  */
        idx_t r = (idx_t)(f % mod);

        if (!seen[r]) {
            /* First time this residue appears.  Remember its value. */
            seen[r] = true;
            first_val[r] = f;
        } else {
            /* Second (or later) time: candidate difference. */
            seq_t diff = f - first_val[r];
            if (diff > (seq_t)0) {
                if (!found || diff < best) {
                    best = diff;
                    found = true;
                }
            }
        }

        /* Prepare factorial for next m, unless we are at the last iteration. */
        if (m < n + 1) {
            f *= (seq_t)(m + 1);
        }
    }

    free(seen);
    free(first_val);
    return best;
}
