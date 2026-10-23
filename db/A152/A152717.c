/*
 * A152717  Triangle T(n,k) read by rows: T(n,k) = 5^min(k, n-k) =
 *          5^A004197(n,k).
 * https://oeis.org/A152717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 5^min(k, r-k) where r = ⌊(√(8n+1)-1)/2⌋ and k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Calculate the row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    double dn = (double)n;
    idx_t r = (idx_t)((sqrt(8.0 * dn + 1.0) - 1.0) / 2.0);

    /* Position of n within this row (0‑based) */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Exponent is the smaller distance to an edge of the row */
    idx_t exp = (k < r - k) ? k : r - k;

    /* Compute 5^exp */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= 5;

    return result;
}
