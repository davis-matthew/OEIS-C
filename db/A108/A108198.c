/*
 * A108198  Triangle read by rows: T(n,k) =
 *          binomial(2k+2,k+1)*binomial(n,k)/(k+2) (0 <= k <= n).
 * https://oeis.org/A108198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = binomial(2k+2,k+1) * binomial(n,k) / (k+2); a(n) is the
 *            triangle read by rows
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }
    /* Column index within the row. */
    idx_t k = n - (r * (r + 1)) / 2;

    /* T(r,k) = Catalan(k+1) * binomial(r,k). */
    seq_t val = catalan(k + 1);
    val *= binom(r, k);
    return val;
}
