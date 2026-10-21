/*
 * A077377  Squarefree numbers whose external digits form a squarefree number.
 * https://oeis.org/A077377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for squarefree integers whose first and last digits
 * jointly form a squarefree number. */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; not used otherwise */

    idx_t found = 0;
    unsigned long long cand = 1;   /* candidate integer */

    for (;; ++cand) {
        /* skip non–squarefree numbers */
        if (!is_squarefree((seq_t)cand))
            continue;

        /* determine first and last digits */
        unsigned long long first = cand;
        while (first >= 10)
            first /= 10;
        unsigned long long last  = cand % 10;

        unsigned long long external = first * 10 + last;

        /* check that the external digits form a squarefree number */
        if (!is_squarefree((seq_t)external))
            continue;

        /* found a valid term */
        ++found;
        if (found == n)
            return (seq_t)cand;
    }
}
