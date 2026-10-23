/*
 * A146987  Triangle, read by rows, T(n, k) = binomial(n, k) for n < 2 and
 *          binomial(n, k) + 3^(n-1)*binomial(n-2, k -1) otherwise.
 * https://oeis.org/A146987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where * r = the largest integer with r(r+1)/2 ≤
 *            n * k = n – r(r+1)/2 (0 ≤ k ≤ r) * T(n,k) = binomial(n,k) if n <
 *            2 * T(n,k) = binomial(n,k) + 3^(n-1) * binomial(n-2,k-1)
 *            otherwise * (binomial(m,t) is 0 if t<0 or t>m, and 3^0 = 1) * *
 *            The triangle is read by rows: row 0 = [1]; row...
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 125, u64 n <= 415,
 *            u128 n <= 1451, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* r grows roughly as sqrt(2n), loop is fast for n ≤ 10000 */
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Position within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Base binomial term */
    seq_t val = binom(r, k);

    /* Add the extra part for r ≥ 2 and k > 0 */
    if (r >= 2 && k > 0) {
        val += pow3(r - 1) * binom(r - 2, k - 1);
    }
    return val;
}
