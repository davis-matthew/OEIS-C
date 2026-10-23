/*
 * A105507  Numbers m such that 7 is the leading digit of the n-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit 7 index of Fibonacci numbers (OEIS A105507).  
   a(n) is the nth non‑negative integer m (n ≥ 1) such that the first
   decimal digit of the Fibonacci number F(m) is 7. */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
/* constants for the logarithms */
static const double LOG10PHI   = 0.20898764024997873;   /* log10((1+√5)/2) */
static const double LOG10SQRT5 = 0.3494850021680094;    /* log10(√5) */

/* test whether the leading decimal digit of the k‑th Fibonacci number is 7 */
static bool leading_digit_is_7(idx_t k)
{
    /* The Binet approximation does not work for k = 0 or 1 */
    if (k <= 1) return false;

    double x = (double)k * LOG10PHI - LOG10SQRT5;
    /* keep fractional part in [0,1) */
    x -= floor(x);
    double v = pow(10.0, x);
    /* floor(10^x) gives the most significant one‑digit block as an integer.
       We need the first decimal digit: */
    int d = (int)v;
    if (d >= 10) d /= 10;
    return d == 7;
}

/* return the n-th such index (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        if (leading_digit_is_7(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
