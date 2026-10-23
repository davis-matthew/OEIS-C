/*
 * A156581  Triangle T(n, k, m) = (m+2)^(k*(n-k)) with m = 15, read by rows.
 * https://oeis.org/A156581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k,15) read by rows,
 *            where T(n,k,15) = 17^(k*(n-k)). The index n (offset 0) is
 *            converted to its (row, column) coordinates by solving
 *            row*(row+1)/2 ≤ n < (row+1)*(row+2)/2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row such that (row+1)*(row+2)/2 > n >= row*(row+1)/2. */
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n) {
        row++;
    }

    /* Number of terms before this row. */
    idx_t before = row * (row + 1) / 2;

    /* Column index within the row. */
    idx_t k = n - before;

    /* Exponent k*(row-k). */
    idx_t exp = k * (row - k);

    /* Compute 17^exp using exponentiation by squaring. */
    seq_t result = 1;
    seq_t base   = (seq_t)17;
    idx_t e = exp;
    while (e) {
        if (e & 1) {
            result *= base;
        }
        base *= base;
        e >>= 1;
    }

    return result;
}
