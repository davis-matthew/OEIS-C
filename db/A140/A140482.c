/*
 * A140482  a(n) = 2*n + tau(n).
 * https://oeis.org/A140482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + tau(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* divisor count τ(n) */
    if (n == 0)          /* only for safety; n >= 1 in practice */
        return (seq_t)0;

    seq_t tau_val = 1;
    idx_t temp = n;

    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            int exponent = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exponent;
            }
            tau_val *= (exponent + 1);
        }
    }
    if (temp > 1) {          /* last prime factor */
        tau_val *= 2;
    }

    seq_t two_n = (seq_t)n * 2;   /* cast before arithmetic */
    return two_n + tau_val;
}
