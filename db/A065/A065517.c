/*
 * A065517  Numerator of n/(sum of the digits of n).
 * https://oeis.org/A065517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n / gcd(n, sum_of_digits(n))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"


/* Return the sum of the decimal digits of n (idx_t = uint64_t). */
static unsigned digit_sum_idx(idx_t n)
{
    unsigned sum = 0;
    while (n) {
        sum += (unsigned)(n % 10U);
        n /= 10U;
    }
    return sum;
}

/* Computes the numerator of n divided by the sum of the digits of n. */
seq_t a(idx_t n)
{
    /* n ≥ 1 due to OEIS offset */
    seq_t N = (seq_t)n;          /* Convert index to seq_t type */
    unsigned s = digit_sum_idx(n);   /* Sum of decimal digits */
    seq_t G = gcd(N, (seq_t)s);   /* gcd provided by utils/gcd.h */
    return N / G;                /* Numerator of the reduced fraction */
}
