/*
 * A061982  a(n) = 3^n - (n+1)*(n+2)/2.
 * https://oeis.org/A061982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - (n+1)*(n+2)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* Compute 3^n using the provided exponentiation routine. */
    seq_t pow = _pow_seq_t((seq_t)3, n);

    /* Compute the binomial term (n+2 choose 2) = (n+1)*(n+2)/2. */
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = n1 + 1;           /* n2 = n+2 */
    seq_t binom = n1 * n2 / 2;   /* exact division by 2 */

    /* Return the difference. */
    return pow - binom;
}
