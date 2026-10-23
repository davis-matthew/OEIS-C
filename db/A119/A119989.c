/*
 * A119989  a(1)=1. a(n) = number of terms among the first (n-1) terms of the
 *          sequence which are coprime to n*(n+1)/2.
 * https://oeis.org/A119989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms among the first n‑1 terms of the sequence
 *            which are coprime to n*(n+1)/2. Here a(1)=1 by definition
 * Range:     u8 n <= 23, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* array of computed a(i) values   */
    static idx_t max_computed = 0;     /* highest index computed so far   */
    static idx_t alloc_cap = 0;        /* current allocation size        */

    /* Ensure we have at least one slot (index 1). */
    if (seq == NULL) {
        alloc_cap = (n > 1 ? n : 1);
        seq = (seq_t*)malloc((alloc_cap + 1) * sizeof(seq_t));
        if (!seq) return (seq_t)0;     /* allocation failed */
        seq[0] = (seq_t)0;             /* unused */
        seq[1] = (seq_t)1;             /* a(1) */
        max_computed = 1;
    }

    /* Grow the array if necessary. */
    if (alloc_cap < n) {
        alloc_cap = n;
        seq = (seq_t*)realloc(seq, (alloc_cap + 1) * sizeof(seq_t));
        if (!seq) return (seq_t)0;     /* reallocation failed */
    }

    /* Compute any missing terms up to n. */
    if (max_computed < n) {
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            seq_t tri = (seq_t)m * ((seq_t)m + 1) / 2;
            seq_t c = 0;
            for (idx_t k = 1; k < m; ++k) {
                if (gcd(seq[k], tri) == (seq_t)1) {
                    ++c;
                }
            }
            seq[m] = c;
        }
        max_computed = n;
    }

    return seq[n];
}
