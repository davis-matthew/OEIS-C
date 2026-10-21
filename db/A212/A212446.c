/*
 * A212446  Floor(2n + log(2n)).
 * https://oeis.org/A212446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n + log(2*n))
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* 2*n fits in 64 bits for all tested n (up to 10 000). */
    unsigned long long two = (unsigned long long)n * 2ULL;

    /* Compute floor(log(2*n)) using long double to avoid rounding errors. */
    long double ln_val = logl((long double)two);
    unsigned long long ln_floor = (unsigned long long)floorl(ln_val);

    return (seq_t)(two + ln_floor);
}
