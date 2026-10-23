/*
 * A106513  A Pell-Pascal matrix.
 * https://oeis.org/A106513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   * a(n) = T(row, col) * where row = floor((sqrt(8*n+1)-1)/2), *
 *            col = n - row*(row+1)/2 * and T(row, col) =
 *            Σ_{j=0..floor((row+1)/2)} binomial(row+1, 2*j+col+1) * 2^j * *
 *            T(row,0) equals the Pell number P_{row+1} (A000129). For k>0 the
 *            above sum is used
 * Range:     u8 n <= 27, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, get_pell, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/get_pell.h"

seq_t a(idx_t n)
{
    /* Find the row 'r' and column 'k' corresponding to the linear index n */
    idx_t r = 0;
    /* (r+1)*(r+2)/2 is the first index of the next row */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* If we are on the first column, return the Pell number */
    if (k == 0)
        return get_pell(r + 1);

    /* Compute the sum using the binomial formula */
    seq_t sum = 0;
    idx_t n_plus1 = r + 1;          /* n in the formula is the row index */
    for (idx_t j = 0; ; ++j)
    {
        idx_t m = 2 * j + k + 1;    /* binomial parameter */
        if (m > n_plus1)
            break;                 /* no further terms contribute */

        seq_t term = binom(n_plus1, m) * pow2(j);
        sum += term;
    }
    return sum;
}
