/*
 * A173077  Triangle T(n, k, q) = binomial(n, k) - 1 +
 *          q^floor(n/2)*binomial(n-2, k-1) with T(n, 0, q) = T(n, n, q) = 1
 *          and q = 3, read by rows.
 * https://oeis.org/A173077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 81, u32 n <= 239, u64 n <= 797,
 *            u128 n <= 1325, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: Triangle T(n, k) = binomial(n, k) - 1 + 3^⌊n/2⌋ * binomial(n-2, k-1)
   with T(n,0)=T(n,n)=1 (q = 3).  The sequence a(n) is obtained by reading the
   triangle row by row (row 0, then row 1, etc.). */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* find the row r that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t prev_total = r * (r + 1) / 2;   /* number of terms before row r */
    idx_t k = n - prev_total;            /* position within the row */

    /* boundary values */
    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    /* compute powers and binomials */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(r / 2));
    seq_t bin_n_k   = binomial(r, k);
    seq_t bin_n2_k1 = binomial(r - 2, k - 1);   /* r >= 2 guaranteed here */

    seq_t result = bin_n_k - 1 + pow3 * bin_n2_k1;
    return result;
}
