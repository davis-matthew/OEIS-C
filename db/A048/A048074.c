/*
 * A048074  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..K} C(floor((n+2)/3), k) * C(n - floor((n+2)/3),
 *            3k) where K = min(floor((n+2)/3), floor((n - floor((n+2)/3))/3))
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* number of elements <= (n+2)/3 */
    idx_t t = (n + 2) / 3;
    /* number of elements > (n+2)/3 */
    idx_t upper = n - t;

    /* maximum k such that k <= t and 3k <= upper */
    idx_t kmax1 = t;
    idx_t kmax2 = upper / 3;
    idx_t kmax = (kmax1 < kmax2) ? kmax1 : kmax2;

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t c1 = binom(t, k);
        seq_t c2 = binom(upper, 3 * k);
        sum += c1 * c2;
    }
    return sum;
}
