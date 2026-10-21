/*
 * A227726  a(n) = [x^n] (1 + x)/(1 - x)^(2*n+1).
 * https://oeis.org/A227726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n) = 4 * binomial(3*n-1, n-1) for n >= 1 (or
 *            equivalently a(n) = binomial(3*n, n) + binomial(3*n-1, n-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    /* compute binomial(3*n-1, n-1) without intermediate overflow */
    idx_t N = 3 * n - 1;                  /* top of the binomial */
    idx_t K = n - 1;                      /* bottom */

    /* Reduce K to min(K, N-K) to minimise loop iterations */
    if (K > N - K)
        K = N - K;

    seq_t res = 1;                        /* running product */

    for (idx_t i = 1; i <= K; ++i) {
        seq_t num = N - K + i;            /* numerator factor  */
        seq_t den = i;                    /* denominator factor */

        /* Cancel common factors between numerator and denominator */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Cancel factors of den with the running product 'res' */
        g = gcd(res, den);
        res /= g;
        den /= g;

        /* Multiply the remaining numerator into 'res' */
        res *= num;

        /* If any factor of the denominator remains, divide it out */
        if (den != 1)
            res /= den;
    }

    /* a(n) = 4 * binomial(...) */
    return res * 4;
}
