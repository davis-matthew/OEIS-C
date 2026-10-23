/*
 * A048071  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(⌊(n+2)/3⌋, n−⌊(n+2)/3⌋)} C(⌊(n+2)/3⌋, k) *
 *            C(n−⌊(n+2)/3⌋, k) (C(a,b) is the binomial coefficient.)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t t = (n + 2) / 3;      /* floor((n+2)/3)          */
    idx_t s = n - t;            /* remaining elements      */
    idx_t limit = t < s ? t : s;
    seq_t result = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t c1 = binomial(t, k);
        seq_t c2 = binomial(s, k);
        result += c1 * c2;
    }
    return result;
}
