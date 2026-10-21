/*
 * A205792  Least positive integer j such that n divides k^5-j^5, where k, as
 *          in A205791, is the least number for which there is such a j.
 * https://oeis.org/A205792
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

/*  A205792:  for each n ≥ 1, a(n) is the least positive integer j
 *  such that n | k^5 - j^5, where k is the smallest integer for which
 *  such a j exists.  The algorithm finds the first repeated residue
 *  in the sequence k^5 (mod n); the earlier index of that residue is
 *  the required j.
 */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* array that records the first occurrence of each residue 0 … n-1  */
    idx_t *first = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    if (!first)          /* memory failure – unlikely */
        return 0;

    idx_t limit = n + 1;                 /* k runs from 1 to n+1 */

    for (idx_t k = 1; k <= limit; ++k) {
        /* r = k^5 mod n  (r < n) */
        seq_t r = mod_pow((seq_t)k, 5ULL, (seq_t)n);
        idx_t res = (idx_t)r;           /* safe cast: r < n */

        if (first[res] != 0) {          /* a previous j exists */
            seq_t j = (seq_t)first[res];
            free(first);
            return j;
        }

        /* record first occurrence; first indices are 1‑based */
        if (k <= n)
            first[res] = k;
    }

    /* by the pigeonhole principle we must have returned earlier */
    free(first);
    return 0;
}
