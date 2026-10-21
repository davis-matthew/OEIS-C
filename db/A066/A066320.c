/*
 * A066320  Triangle read by rows: T(n, k) = binomial(n, k)*k^k*(n-k)^(n-k-1)
 *          k=0..n-1.
 * https://oeis.org/A066320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the row‑major triangle whose entries
 *            are T(r,k) = binomial(r,k) * k^k * (r-k)^(r-k-1), with k =
 *            0..r-1. (OEIS A066320)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1, but return 0 */

    /* Find the row r that contains the n-th term.
       The number of terms up to row r is r*(r+1)/2.    */
    idx_t r = 1;
    idx_t sum = 1;                  /* sum = r*(r+1)/2 for current r */
    while (sum < n) {
        r++;
        sum += r;
    }

    /* First index of this row */
    idx_t start_of_row = sum - r + 1;
    /* Column index within the row (0‑based) */
    idx_t k = n - start_of_row;

    /* Compute the triangle entry: binomial(r,k) * k^k * (r-k)^(r-k-1) */
    seq_t term = binomial(r, k);
    seq_t pow_k  = big_pow((seq_t)k,          (seq_t)k);
    seq_t pow_rk = big_pow((seq_t)(r - k),    (seq_t)(r - k - 1));

    term *= pow_k;
    term *= pow_rk;
    return term;
}
