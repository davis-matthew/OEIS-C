/*
 * A204996  Least k^2 such that n divides k^2-j^2 for some j<k.
 * https://oeis.org/A204996
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

/* Sequence A204996
   a(n) = least k² such that ∃ j < k with n | (k² – j²).
   Algorithm: iterate k = 1,2,…, keep a boolean array of residues
   of k² mod n seen so far. The first time a residue repeats,
   we have found j < k with equal residues, so the current k² is
   the desired value. Pigeonhole principle guarantees a repeat
   within at most n+1 steps.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is 1‑based in OEIS, but we treat 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Allocate an array to track seen residues modulo n. */
    size_t cap = (size_t)n;
    bool *seen = (bool *)calloc(cap, sizeof(bool));
    if (!seen)
        return 0;          /* allocation failure – unlikely for small n */

    for (idx_t k = 1; ; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        idx_t r = (idx_t)(sq % (seq_t)n);   /* residue modulo n */
        if (seen[r]) {                      /* repeat found */
            free(seen);
            return sq;
        }
        seen[r] = true;

        /* By pigeonhole the loop will terminate before k > n+1. */
        if (k >= n + 1) {
            free(seen);
            return sq;
        }
    }
}
