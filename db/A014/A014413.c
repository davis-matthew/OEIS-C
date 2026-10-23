/*
 * A014413  Triangular array formed from elements to right of middle of
 *          Pascal's triangle.
 * https://oeis.org/A014413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = the binomial coefficient corresponding to the element on
 *            the right side of Pascal's triangle row r, where r is chosen
 *            according to the sequence indexing. The sequence skips row r = 2
 *            of Pascal's triangle. The first two terms are a(1)=1 (r=0) and
 *            a(2)=1 (r=1). For n ≥ 3, let r start at 3 an...
 * Range:     u8 n <= 20, u16 n <= 90, u32 n <= 306, u64 n <= 1156,
 *            u128 n <= 4356, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* External linkage – required by the framework */
seq_t a(idx_t n)
{
    /* Special cases for the first two terms */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* Number of terms remaining after the first two */
    idx_t remaining = n - 2;
    /* Start scanning rows from r = 2; skip r = 2 later */
    idx_t r = 2;

    while (true) {
        if (r == 2) {      /* skip row 2 of Pascal's triangle */
            r++;
            continue;
        }

        /* Number of elements on the right half of row r */
        idx_t l = r - (r / 2);

        if (remaining <= l) {
            /* Position j within this row (zero‑based) */
            idx_t j = remaining - 1;
            /* Column index k for binomial coefficient */
            idx_t k = (r / 2) + 1 + j;
            return binom(r, k);
        }

        remaining -= l;
        r++;
    }
}
