/*
 * A080290  a(n) = n^n * (n + 2 - (1/12) * (n^2-1)).
 * https://oeis.org/A080290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^n * (n + 2 - (n^2-1)/12)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n^n * (-n^2 + 12*n + 25) / 12  */

#include <stdint.h>   /* for idx_t (uint64_t) */

static seq_t pow_n_n(idx_t n)
{
    seq_t result = 1;
    seq_t base   = (seq_t)n;
    idx_t exp    = n;
    while (exp > 0)
    {
        if (exp & 1UL)
            result *= base;
        if (exp == 1U)
            break;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n==0 but handled */
        return 0;

    /* n^n */
    seq_t power = pow_n_n(n);

    /* coefficient: -n^2 + 12*n + 25  */
    seq_t coef = -((seq_t)n * (seq_t)n) + (seq_t)12 * (seq_t)n + (seq_t)25;

    /* product and exact division by 12 */
    seq_t prod = power * coef;
    return prod / (seq_t)12;
}
