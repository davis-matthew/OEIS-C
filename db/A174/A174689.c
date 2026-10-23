/*
 * A174689  Triangle T(n, k) = n! * binomial(n, k)^2 - n! + 1, read by rows.
 * https://oeis.org/A174689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col) where T(row, col) = row! * (C(row, col)^2 -
 *            1) + 1 Flattened by rows of the triangle
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 125,
 *            u128 n <= 336, big any n
 * Helpers:   binomial, factorial, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/offset_row.h"
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Computes the n-th term of OEIS sequence A174689.  */
seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        r++;

    /* Column index within the row. */
    idx_t offset = offset_row(r);
    idx_t k = n - offset;

    /* Evaluate T(r, k) = r! * C(r,k)^2 - r! + 1. */
    seq_t fact = factorial(r);
    seq_t bin  = binomial(r, k);
    seq_t inner = bin * bin - 1;          /* C(r,k)^2 - 1 */
    seq_t result = fact * inner + 1;

    return result;
}
