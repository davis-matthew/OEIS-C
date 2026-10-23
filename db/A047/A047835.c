/*
 * A047835  a(n) = Product_{i=1..n}
 *          ((i+4)*(i+5)*(i+6)*(i+7))/(i*(i+1)*(i+2)*(i+3)).
 * https://oeis.org/A047835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} ((i+4)*(i+5)*(i+6)*(i+7)) / ( i
 *            *(i+1)*(i+2)*(i+3) ) (offset 0)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 10, u64 n <= 53, u128 n <= 922,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Euclidean GCD for seq_t.  Defined as a static helper. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Computes the n-th term of OEIS A047835. */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Build numerator and denominator for the current term. */
        seq_t num = (seq_t)(i + 4);
        num *= (seq_t)(i + 5);
        num *= (seq_t)(i + 6);
        num *= (seq_t)(i + 7);

        seq_t den = (seq_t)i;
        den *= (seq_t)(i + 1);
        den *= (seq_t)(i + 2);
        den *= (seq_t)(i + 3);

        /* Reduce the rational factor by cancelling common factors. */
        seq_t g = gcd_seq(num, den);
        if (g != (seq_t)1) {
            num /= g;
            den /= g;
        }

        /* Cancel factors between current result and the denominator. */
        g = gcd_seq(res, den);
        if (g != (seq_t)1) {
            res /= g;
            den /= g;
        }

        /* Multiply by the reduced numerator and divide by the reduced denominator. */
        res *= num;
        /* Division is exact due to the cancellation above. */
        res /= den;
    }

    return res;
}
