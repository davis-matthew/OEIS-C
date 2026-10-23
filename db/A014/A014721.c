/*
 * A014721  Squares of elements to left of the central element in Pascal
 *          triangle (by row).
 * https://oeis.org/A014721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by enumerating the binomial coefficients * that
 *            lie to the left of the central element in each row of Pascal's *
 *            triangle (rows indexed from 1). For row r the left‑side
 *            coefficients * are C(r, k) with k ranging from floor((r-1)/2)
 *            down to 0. The * sequence is the square of each o...
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)                     /* sequence is 1‑indexed */
        return 0;

    /* Find the row r that contains the n‑th term. */
    idx_t total = 0;                /* total terms counted so far */
    idx_t r = 1;                    /* current row (1‑based) */

    /* Each row r contributes t(r) = floor((r-1)/2) + 1 terms. */
    while (total + ((r - 1) / 2 + 1) < n) {
        total += (r - 1) / 2 + 1;
        ++r;
    }

    /* Position within the current row (1‑based). */
    idx_t p = n - total;

    /* Compute the binomial coefficient index k. */
    idx_t k_start = (r - 1) / 2;
    idx_t k = k_start - (p - 1);    /* k >= 0 guaranteed */

    /* Compute the square of the coefficient. */
    seq_t coeff = binom(r, k);
    return coeff * coeff;
}
