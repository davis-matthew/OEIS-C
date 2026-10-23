/*
 * A192442  Coefficient of x^n in the expansion of (1+x^3+x^4)^n.
 * https://oeis.org/A192442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 115,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{j = ⌊n/4⌋}^{⌊n/3⌋} binom(j, n-3j) * binom(n, j)
 * This computes the coefficient of x^n in (1 + x^3 + x^4)^n.
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t lo = n / 4;          /* lower bound of j   */
    idx_t hi = n / 3;          /* upper bound of j   */

    for (idx_t j = lo; j <= hi; ++j) {
        idx_t k = n - 3 * j;          /* n - 3*j  */
        seq_t t1 = binom(j, k);       /* binom(j, n-3j)   */
        seq_t t2 = binom(n, j);       /* binom(n, j)      */
        result += t1 * t2;
    }
    return result;
}
