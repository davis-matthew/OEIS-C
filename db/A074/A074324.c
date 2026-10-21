/*
 * A074324  a(2n+1) = 3^n, a(2n) = 4*3^(n-1) except for a(0) = 1.
 * https://oeis.org/A074324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 3^{(n‑1)/2} if n is odd, a(n) = 4·3^{(n/2)‑1}
 *            if n is even and n > 0
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   pow3_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3_exp.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n & 1) {                        // odd n
        idx_t k = (n - 1) / 2;
        return pow3_exp(k);
    } else {                            // even n > 0
        idx_t k = n / 2;
        seq_t p = pow3_exp(k - 1);
        return (seq_t)4 * p;
    }
}
