/*
 * A056566  Fibonomial coefficients.
 * https://oeis.org/A056566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonomial(n+8,8) = Π_{i=1}^8 Fib(n+i) / 65520
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Denominator factors of 65520 = Π_{i=1}^8 Fib(i) */
    static const unsigned int denom_fibs[8] = {1, 1, 2, 3, 5, 8, 13, 21};

    /* Mutable copy of denominator factors */
    seq_t den[8];
    for (int j = 0; j < 8; ++j)
        den[j] = denom_fibs[j];

    seq_t product = 1;

    for (idx_t i = 1; i <= 8; ++i) {
        seq_t val = fib(n + i);          /* Fib(n+i) */

        /* Cancel common factors with the current denominator pieces */
        for (int j = 0; j < 8; ++j) {
            if (den[j] == (seq_t)1) continue;
            seq_t g = gcd(val, den[j]);
            if (g > (seq_t)1) {
                val  /= g;
                den[j] /= g;
            }
        }

        product *= val;
    }

    /* Any remaining denominator factors must divide the product exactly */
    seq_t remaining = 1;
    for (int j = 0; j < 8; ++j)
        remaining *= den[j];

    return product / remaining;
}
