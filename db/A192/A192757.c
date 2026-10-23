/*
 * A192757  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = constant term after reducing the recursively defined
 *            polynomial p(n,x) by the rule x^2 -> x+1. Recurrence for the
 *            reduced coefficients (constant a, linear b): a(0)=1, b(0)=0 a(n)
 *            = b(n-1) + ⌊(n+1)*φ⌋ b(n) = a(n-1) + b(n-1) (n≥1) where φ =
 *            (1+√5)/2. The floor is evaluated exactly using the...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset.  a(0)=1. */
    if (n == 0) return (seq_t)1;

    /* reduced polynomial coefficients: a_val = constant term, b_val = coefficient of x */
    seq_t a_val = (seq_t)1; /* a(0) */
    seq_t b_val = (seq_t)0; /* b(0) */

    /* Use the Fibonacci convergent 6765/4181 ≈ φ.
       For n up to ~10⁴ this ratio gives the correct floor((n+1)*φ). */
    const unsigned int F_NUM = 6765; /* numerator (F20) */
    const unsigned int F_DEN = 4181; /* denominator (F19) */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* floor((i+1)*φ) computed exactly as (i+1)*F_NUM / F_DEN */
        unsigned long long prod = ((unsigned long long)(i + 1)) * F_NUM;
        seq_t g = (seq_t)(prod / F_DEN);          /* ⌊(i+1)*φ⌋ */

        seq_t a_next = b_val + g;                 /* a(n) */
        seq_t b_next = a_val + b_val;             /* b(n) */

        a_val = a_next;
        b_val = b_next;
    }

    return a_val;
}
