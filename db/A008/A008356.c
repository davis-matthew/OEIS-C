/*
 * A008356  Crystal ball sequence for D_5 lattice.
 * https://oeis.org/A008356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x)*(1+34x+146x^2+34x^3+x^4)/(1-x)^6 This expands to a(n) =
 *            Σ_{k=0..5} C_k * binom(n-k+5,5) with C = {1, 35, 180, 180, 35,
 *            1}
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* multiplicative binomial coefficient binom(m,5) for m >= 5; 0 for m < 5 */
static seq_t binom5(idx_t m)
{
    if (m < 5) return 0;
    seq_t r = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        seq_t num = (seq_t)(m - 5 + i);   /* terms m-4, m-3, …, m   */
        seq_t den = (seq_t)i;             /* 1,2,…,5               */
        seq_t g   = gcd(num, den);       /* reduce to keep r small */
        num /= g;
        den /= g;
        r *= num;
        r /= den;
    }
    return r;
}

seq_t a(idx_t n)
{
    static const seq_t coeff[6] = {1, 35, 180, 180, 35, 1};
    seq_t result = 0;
    for (idx_t k = 0; k < 6; ++k) {
        if (n < k) continue;
        /* binom(n-k+5,5) */
        seq_t bin = binom5(n - k + 5);
        result += coeff[k] * bin;
    }
    return result;
}
