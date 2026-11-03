/*
 * A131137  Denominator of (exponential) expansion of log((2*x/3-1)/(x-1)).
 * https://oeis.org/A131137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 * a(n) = 3^{(n+1+s_3(n-1))/2} for n >= 1 *
 *            where s_3(m) is the sum of the base‑3 digits of m
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 74, u128 n <= 152,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 199 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow3.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();          /* a(0) = 1 */

    idx_t m = n - 1;                 /* compute s_3(n-1) */
    unsigned sum = 0;
    while (m) {
        sum += m % 3;
        m /= 3;
    }

    idx_t exponent = (n + 1 + sum) / 2;
    return pow3(exponent);
}
