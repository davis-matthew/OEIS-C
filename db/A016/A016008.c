/*
 * A016008  a(n) = (tau(n^12)+11)/12.
 * https://oeis.org/A016008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n^12)+11)/12
 * Range:     u8 n <= 11, u16 n <= 9239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined for offset 1, safeguard */
    }
    idx_t temp = n;
    seq_t tau = 1;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            tau *= (seq_t)(12 * e + 1);
        }
    }
    if (temp > 1) {          /* remaining prime factor with exponent 1 */
        tau *= (seq_t)13;    /* 12*1 + 1 */
    }
    seq_t result = (tau + (seq_t)11) / (seq_t)12;
    return result;
}
