/*
 * A123072  Bishops on an 8n+1 X 8n+1 board (see Robinson paper for details).
 * https://oeis.org/A123072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-1) * (2*n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* compute (2*n)! */
    idx_t double_n = n + n; /* 2*n fits in idx_t (uint64_t) */
    seq_t fact = factorial(double_n);

    /* compute binomial(2*n-1, n-1) */
    idx_t two_minus_one = double_n - (idx_t)1;
    seq_t bin = binomial(two_minus_one, n - (idx_t)1);

    /* multiply them together */
    return seq_mul(fact, bin);
}
