/*
 * A113335  a(n) = 3^5 * binomial(n+4, 5).
 * https://oeis.org/A113335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 243 * binomial(n+4, 5)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0)                 /* defined for offset completeness */
        return 0;
    idx_t N = n + 4;             /* upper parameter of the binomial */
    seq_t res = 1;
    for (idx_t k = 1; k <= 5; ++k) {
        /* multiplicative binomial: res = res * (N - k + 1) / k */
        res = res * (seq_t)(N - k + 1) / k;
    }
    return 243 * res;            /* final factor 3^5 = 243 */
}
