/*
 * A038268  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*2^j.
 * https://oeis.org/A038268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 7^(i-j) * 2^j where i and j are the row
 *            and column indices of n when the triangle T(i,j) = binomial(i,j)
 *            * 7^(i-j) * 2^j is read row‑wise
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Find the row i such that n is in [i*(i+1)/2 , (i+1)*(i+2)/2 - 1] */
    idx_t i = 0;
    for (;;) {
        idx_t next = ((i + 1) * (i + 2)) / 2;
        if (n < next)
            break;
        ++i;
    }
    idx_t prev = (i * (i + 1)) / 2;
    idx_t j = n - prev;          /* column index within the row */

    /* Compute the triangle entry */
    seq_t term = binomial(i, j);
    term *= ipow((seq_t)7, (seq_t)(i - j));
    term *= ipow((seq_t)2, (seq_t)j);

    return term;
}
