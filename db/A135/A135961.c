/*
 * A135961  G.f.: A(x) = Sum_{n>=0} x^n/(1 - Fibonacci(n)*x).
 * https://oeis.org/A135961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Fibonacci(k)^(n-k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 20, u128 n <= 28,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Arithmetic working on Fibonacci numbers lazily. */
    seq_t f0 = (seq_t)0;      /* F(k-2) */
    seq_t f1 = (seq_t)1;      /* F(k-1) */

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base;
        if (k == 0) {
            base = f0;           /* F(0) = 0 */
        }
        else if (k == 1) {
            base = f1;           /* F(1) = 1 */
        }
        else {
            /* Compute F(k) = F(k-2) + F(k-1) and advance the window. */
            base = f0 + f1;
            f0   = f1;
            f1   = base;
        }

        /* The exponent is (n - k). */
        idx_t exp = n - k;

        seq_t term;
        if (exp == 0) {
            /* Any base^0 is defined as 1 (including 0^0). */
            term = (seq_t)1;
        }
        else if (base == (seq_t)0) {
            /* 0 raised to a positive power is 0. */
            term = (seq_t)0;
        }
        else {
            term = seq_pow(base, exp);
        }

        sum += term;
    }

    return sum;
}
