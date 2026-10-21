/*
 * A203469  a(n) = v(n)/A000178(n), v = A093883 and A000178 =
 *          (superfactorials).
 * https://oeis.org/A203469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} binomial(2*n - i , i)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* sequence is defined for n ≥ 1 */
    seq_t result = 1;             /* start product at 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* binomial(2*n - i , i) – all arguments fit in idx_t */
        seq_t term = binomial((idx_t)(2 * n - i), (idx_t)i);
        result = result * term;    /* multiply into running product */
    }

    return result;
}
