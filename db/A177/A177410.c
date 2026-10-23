/*
 * A177410  a(n) = binomial((n+1)*2^n, n)/(n+1).
 * https://oeis.org/A177410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial((n+1)*2^n, n)/(n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute m = (n+1) * 2^n using arbitrary‑precision arithmetic. */
    seq_t m = pow2(n) * (seq_t)(n + 1);

    seq_t result = 1;  /* binomial value for k = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* result = result * (m - k + 1) / k  */
        result = result * (m - (k - 1));
        result = result / k;
    }

    /* Divide by (n+1) to obtain a(n). */
    return result / (seq_t)(n + 1);
}
