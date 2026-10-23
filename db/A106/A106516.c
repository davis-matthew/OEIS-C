/*
 * A106516  A Pascal-like triangle based on 3^n.
 * https://oeis.org/A106516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = T(r,k) where
 *     r = ⌊(√(8n+1)-1)/2⌋            (row index such that r(r+1)/2 ≤ n < (r+1)(r+2)/2)
 *     k = n - r(r+1)/2                (column index within the row)
 *     T(0,0)      = 1
 *     T(n,0)      = 3^n
 *     T(n,n)      = 1
 *     T(n,k)      = T(n-1,k-1) + T(n-1,k)   for 1 ≤ k ≤ n-1
 *   (The same values can also be produced by the closed form
 *    T(n,k) = binomial(n,k) + 2 * Σ_{i=1}^{n} 3^{i-1} * binomial(n-i,k).)
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/row_of.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Determine the row (r) and column (k) that contain the answer. */
    idx_t r = row_of(n);                     /* r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t k = n - r * (r + 1) / 2;          /* offset within the row */

    /* Compute T(r,k) = binom(r,k) + 2 * Σ_{i=1}^{r} 3^{i-1} * binom(r-i,k). */
    seq_t bin = binom(r, k);                 /* binomial(r,k) */
    seq_t sum = 0;
    seq_t pow3 = 1;                          /* 3^0 */

    for (idx_t i = 1; i <= r; ++i) {
        seq_t c = binom(r - i, k);           /* binomial(r-i,k) */
        sum += pow3 * c;                     /* add 3^{i-1} * binomial(r-i,k) */
        pow3 *= 3;                          /* next power of 3 (3^{i}) */
    }

    seq_t res = bin + (sum << 1);            /* + 2*sum */
    return res;
}
