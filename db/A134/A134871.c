/*
 * A134871  a(1) = 1, a(n) = tau(n) + n - 2 for n > 1.
 * https://oeis.org/A134871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + n - 2 for n > 1, a(1) = 1
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* sequence defined only for n ≥ 1 */
    if (n == 1)
        return 1;

    /* factorise n to compute τ(n) */
    idx_t temp = n;
    seq_t tau = 1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            tau *= (seq_t)(e + 1);
        }
    }
    if (temp > 1) {              /* remaining prime factor */
        tau *= (seq_t)2;
    }

    return tau + (seq_t)n - (seq_t)2;
}
