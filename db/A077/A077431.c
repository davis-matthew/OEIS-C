/*
 * A077431  n repeated in decimal representation, but separated by enough
 *          zeros that the square has the pattern (n^2)(2n^2)(n^2).
 * https://oeis.org/A077431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*(1+10^(1+floor(log10(2*n^2))))
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute n*(1 + 10^digits(2*n^2)) */
    /* Note: for any n>=1 the expression 10^digits(2*n^2) equals 10^(1+floor(log10(2*n^2))) */

    if (n == 0) {
        return (seq_t)0;
    }

    seq_t nn   = (seq_t)n;     /* convert index to seq_t   */
    seq_t prod = nn * nn;      /* n^2                           */
    prod *= 2;                /* 2*n^2                         */

    /* Count decimal digits of prod (digits = floor(log10(prod)) + 1) */
    unsigned long long digits = 0;
    seq_t temp = prod;
    if (temp == 0) {
        digits = 1;
    } else {
        while (temp > 0) {
            temp /= 10;
            digits++;
        }
    }

    /* 10^digits */
    seq_t power = 1;
    for (unsigned long long i = 0; i < digits; ++i) {
        power *= 10;
    }

    /* Result: n * (power + 1) */
    return nn * (power + (seq_t)1);
}
