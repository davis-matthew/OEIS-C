/*
 * A026321  n-th nonnegative integer k satisfying |sin(k)| > |cos(k)| >
 *          |sin(k+1)|.
 * https://oeis.org/A026321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th nonnegative integer k such that
   |sin(k)| > |cos(k)| > |sin(k+1)|.  The function searches
   k = 0, 1, 2, … until the n‑th such k is found. */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Return the n-th term of OEIS A026321.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)              /* behaviour below the offset is irrelevant */
        return 0;

    idx_t k = 0;             /* candidate k */
    idx_t count = 0;         /* how many good k's found so far */

    while (1) {
        double sk   = sin((double)k);
        double ck   = cos((double)k);
        double sk1  = sin((double)(k + 1));

        if (fabs(sk) > fabs(ck) && fabs(ck) > fabs(sk1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
