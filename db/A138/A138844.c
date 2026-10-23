/*
 * A138844  Concatenation of initial and final digits of n-th positive
 *          Fibonacci number.
 * https://oeis.org/A138844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the concatenation of the first and last decimal * digits
 *            of the nth Fibonacci number (F₁ = 1, F₂ = 1). The value * thus
 *            lies between 11 and 99. The implementation uses a Pisano *
 *            period modulo 10 for the last digit, and a Binet‑style
 *            logarithmic * approximation for the first digit (exact...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

/* Pre‑computed Fibonacci numbers modulo 10 for one Pisano period
 * (period length 60).  f[i] = Fᵢ mod 10, with F₀ = 0. */
static const unsigned char fib10[60] = {
    0, 1, 1, 2, 3, 5, 8, 3, 1, 4,
    5, 9, 4, 3, 7, 0, 7, 7, 4, 1,
    5, 6, 1, 7, 8, 5, 3, 8, 1, 9,
    0, 9, 9, 8, 7, 5, 2, 7, 9, 6,
    5, 1, 6, 7, 3, 0, 3, 3, 6, 9,
    5, 4, 9, 3, 2, 5, 7, 2, 9, 1
};

/* Compute F_n mod 10 using the Pisano period.  n ≥ 1. */
static seq_t fib_mod_10(idx_t n)
{
    return fib10[n % 60];
}

/* Compute the leading decimal digit of F_n (n ≥ 1).  For n ≤ 94 the
 * exact Fibonacci number is used; for larger n the logarithmic
 * formula is sufficiently accurate. */
static seq_t leading_digit(idx_t n)
{
    if (n <= 94) {
        /* Compute F_n exactly in 64‑bit integer. */
        uint64_t a = 0, b = 1, c;
        while (n--) {
            c = a + b;
            a = b;
            b = c;
        }
        /* a now holds F_n.  Extract first digit. */
        while (a >= 10) a /= 10;
        return (seq_t)a;
    }

    /* Binet‑style approximation to get the first digit. */
    const long double log10_phi = 0.20898764024997873L;   /* log10((1+√5)/2) */
    const long double log10_sqrt5 = 0.3494850021680094L;  /* 0.5*log10(5) */
    long double x = (long double)n * log10_phi - log10_sqrt5;
    long double frac = x - floorl(x);
    long double val = powl(10.0L, frac);
    int digit = (int)val;
    if (digit == 10) digit = 9;  /* guard against rounding error */
    return (seq_t)digit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Last decimal digit from Pisano period. */
    seq_t last = fib_mod_10(n);

    /* First decimal digit. */
    seq_t first = leading_digit(n);

    /* Concatenate the two digits: first * 10 + last. */
    return first * (seq_t)10 + last;
}
