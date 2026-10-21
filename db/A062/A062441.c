/*
 * A062441  Integer part of log(n)^(1 + log(n)).
 * https://oeis.org/A062441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊ (log(n))^(1 + log(n)) ⌋ (natural logarithm)
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Handle the offset and small arguments */
    if (n <= 2)
        return 0;

    /* Natural logarithm of n */
    long double ln_n = logl((long double)n);

    /* (1 + log(n)) * log(log(n)) */
    long double L = (1.0L + ln_n) * logl(ln_n);

    /* Rough estimate of the result */
    long double val = expl(L);
    unsigned long long k = (unsigned long long)floorl(val);

    /* Correct possible rounding errors */
    /* Decrease while the candidate is too large */
    while (k > 0 &&
           logl((long double)k) > L)
        --k;

    /* Increase while the candidate is too small */
    while (logl((long double)(k + 1)) <= L)
        ++k;

    return (seq_t)k;
}
