/*
 * A205551  The least j such that n divides k^k-j^j, where k (as in A205546)
 *          is the least number for which there is such a j.
 * https://oeis.org/A205551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A205551: the least j such that n divides
   k^k - j^j, where k is the smallest integer for which such a j exists.
   The function searches the sequence of m^m mod n for the first repeated
   residue. The earlier occurrence of that residue is the required j. */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for 0, but never invoked with 0 */

    /* first[r] holds the first index m (1‑based) where residue r appeared.
       0 indicates the residue has not been seen yet. */
    idx_t *first = (idx_t*)malloc((size_t)n * sizeof(idx_t));
    if (!first)
        return 0;   /* allocation failure – unlikely for n ≤ 10000 */

    for (idx_t i = 0; i < n; ++i)
        first[i] = 0;

    /* By the pigeonhole principle, among the first n+1 values of m^m mod n
       a residue must repeat. The first duplicate identifies the required j. */
    for (idx_t m = 1; m <= n + 1; ++m) {
        seq_t val = mod_pow((seq_t)m, m, (seq_t)n);   /* m^m mod n */
        idx_t residue = (idx_t)val;                   /* 0 .. n-1 */

        if (first[residue] == 0) {
            first[residue] = m; /* record first occurrence */
        } else {
            seq_t ans = (seq_t)first[residue]; /* earlier index is j */
            free(first);
            return ans;
        }
    }

    /* Should never reach here – a duplicate is guaranteed by m = n+1. */
    free(first);
    return 0;
}
