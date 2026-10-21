/*
 * A068763  Irregular triangle of the Fibonacci polynomials of A011973
 *          multiplied diagonally by the Catalan numbers.
 * https://oeis.org/A068763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = binomial(r + 1 - m, m) * C(r - m) where r is the row
 *            number of the triangle that contains the term a(n) and m is its
 *            zero‑based column within that row. C(k) denotes the k‑th Catalan
 *            number
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
/* helper for binomial coefficients (defined elsewhere in the build) */
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* compute a(n) from A068763 */
seq_t a(idx_t n)
{
    /* --------- find row r and column m for index n --------- */
    idx_t r     = 0;
    idx_t offset = 0;
    while (1) {
        idx_t len = (r + 3) / 2;    /* floor((r+3)/2) */
        if (n < offset + len) break;
        offset += len;
        ++r;
    }
    idx_t m = n - offset;          /* column within the row */
    idx_t cat_idx = r - m;          /* index for Catalan number */

    /* --------- compute a(n) from the triangle formula --------- */
    seq_t bin = binomial(r + 1 - m, m);   /* binomial coefficient   */
    seq_t cat = catalan(cat_idx);         /* Catalan number         */
    return bin * cat;
}
