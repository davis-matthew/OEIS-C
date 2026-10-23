/*
 * A162664  a(n) = sigma(n) + tau(n)^2.
 * https://oeis.org/A162664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + tau(n)^2
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard – not part of OEIS offset */
    seq_t sigma = 0;
    seq_t tau   = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            tau   += 1;
            idx_t j = n / i;
            if (j != i) {
                sigma += (seq_t)j;
                tau   += 1;
            }
        }
    }
    return sigma + tau * tau;
}
