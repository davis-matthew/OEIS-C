/*
 * A055864  Coefficient triangle for certain polynomials.
 * https://oeis.org/A055864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth term (row‑major) of the triangle a(n,m) =
 *            n^(m-1) * (n+1)^(n-m) for 1 ≤ m ≤ n. The index n maps to the
 *            pair (row,col) by n = 1 + 2 + … + (row-1) + col, 1 ≤ col ≤ row
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   _triangle_row, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/_triangle_row.h"

/* Return the nth term of the sequence. */
extern seq_t a(idx_t n)
{
    /* Determine the row r (1‑based) containing the nth term */
    idx_t r = _triangle_row(n);

    /* Sum of 1 + … + (r-1) */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;

    /* Column index within the row (1‑based). */
    idx_t col = n - (idx_t)prev_sum;

    /* Compute n^(m-1) * (n+1)^(n-m) */
    seq_t part1 = big_pow((seq_t)r, (seq_t)(col - 1));
    seq_t part2 = big_pow((seq_t)(r + 1), (seq_t)(r - col));
    return part1 * part2;
}
