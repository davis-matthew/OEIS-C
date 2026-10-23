/*
 * A179099  Rectified 8-simplex numbers: the coefficient of x^(2n-2) in
 *          (1+x+x^2+...+x^(n-1))^9.
 * https://oeis.org/A179099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rectified 8-simplex numbers – OEIS A179099
   Formula: a(0) = 0
            a(n) = Σ_{i=1}^{5} c_i * binom(8 + n - i, 8)   (n ≥ 1),
   where c = {1, 27, 126, 84, 9} and binom(k, 8) = k! / (8! (k-8)!).
   This implements the generating‑function expression
   G(x) = x(1+27x+126x²+84x³+9x⁴)/(1−x)⁹.
*/

#include <stdint.h>
#include "../utils/binom_8.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    static const uint64_t coeff[5] = {1, 27, 126, 84, 9};
    seq_t result = 0;
    for (int i = 1; i <= 5; ++i) {
        if (n < (idx_t)i) continue;          /* avoid unsigned underflow */
        seq_t N = (seq_t)(8 + n - i);          /* 8 + n - i  ≥ 8 if used */
        seq_t bin = binom_8(N);
        result += bin * coeff[i - 1];
    }
    return result;
}
