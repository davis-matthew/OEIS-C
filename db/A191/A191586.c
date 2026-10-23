/*
 * A191586  Binomial row sums of the Riordan matrix (1/(1-x),x/(1-x^2))
 *          (A046854).
 * https://oeis.org/A191586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} binomial(n,k) * binomial(floor((n+k)/2), k)
 * (OEIS A191586)
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binom(n, k);
        idx_t m = (n + k) >> 1;          /* floor((n+k)/2) */
        seq_t c2 = binom(m, k);
        total += c1 * c2;
    }
    return total;
}
