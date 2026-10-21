/*
 * A205388  Least s(k) such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 58, u64 n <= 276, u128 n <= 630,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) from A205388.
   a(n) is the first value s(k) = C(2k,k)/2 that repeats modulo n.
   The function iterates k≥1, uses the provided binom() routine to
   compute s(k) exactly, keeps a table of seen residues modulo n,
   and stops when a repeat occurs.  The returned value is the exact
   multivariate integer s(k).  For n<1 the function returns 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) → seq_t */

seq_t a(idx_t n)
{
    if (n < 1U)          /* sequence undefined for n=0 */
        return (seq_t)0;

    /* Boolean table of residues modulo n (0 … n‑1).  */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen)         /* allocation failure */
        return (seq_t)0;

    idx_t k = 1;       /* start with s(1) */
    while (1) {
        /* Exact value of s(k) = C(2k,k)/2 */
        seq_t val = binom((idx_t)(2 * k), k);
        val = val / (seq_t)2;

        /* Residue of s(k) modulo n */
        seq_t mod_val = val % (seq_t)n;
        idx_t r = (idx_t)mod_val;   /* safe:   mod_val < n <= 10000 */

        if (seen[r]) {              /* repeat found → a(n) = s(k) */
            free(seen);
            return val;
        }
        seen[r] = true;              /* record this residue */

        ++k;
    }
}
