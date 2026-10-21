/*
 * A072354  a(n)-th Fibonacci number is the smallest Fibonacci number
 *          containing n digits.
 * https://oeis.org/A072354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the index of the first Fibonacci number having exactly n
 *            digits. This implementation uses the logarithmic approximation
 *            of the digit count and a small correction loop to ensure
 *            correctness for all n ≥ 1
 * Range:     u8 n <= 53, u16 n <= 9781, u32 n <= 9781, u64 n <= 9781,
 *            u128 n <= 9781, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

#define LOG10_PHI        0.20898764024997873L   /* log10(phi)   */
#define LOG10_SQRT5      0.3494850021680094L    /* log10(sqrt(5)) */

/* Return the number of decimal digits of the nth Fibonacci number (F_1 = 1). */
static idx_t fib_digits(idx_t k)
{
    if (k <= 1)
        return 1;
    long double x = (long double)k * LOG10_PHI - LOG10_SQRT5;
    /* truncation on positive values behaves as floor */
    return (idx_t)(x) + 1;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1U;        /* F_1 is the first 1‑digit Fibonacci number */

    /* Initial estimate using the asymptotic formula */
    long double estimate = ((long double)n - LOG10_SQRT5) / LOG10_PHI;
    idx_t k = (idx_t)ceil(estimate);

    /* Correct possible off–by–one errors caused by rounding */
    while (fib_digits(k) < n)
        ++k;
    while (k > 1 && fib_digits(k - 1) >= n)
        --k;

    return (seq_t)k;
}
