/*
 * A065518  Denominator of n/(sum of the digits of n).
 * https://oeis.org/A065518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_of_decimal_digits(n) / gcd(n,
 *            sum_of_decimal_digits(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the denominator of n/(sum of the digits of n) by simplifying the fraction n / digit_sum(n). */

#include <limits.h>
#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* compute the sum of decimal digits of n */
    uint64_t sum = 0;
    idx_t tmp = n;
    while (tmp > 0)
    {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* n >= 1 for the sequence; avoid division by zero */
    if (sum == 0)
        return (seq_t)0; /* undefined for n==0, but return 0 */

    uint64_t g = gcd_u64((uint64_t)n, sum);

    /* result fits in sum / g <= sum <= 9 * 20 < 200 for 64‑bit n */
    return (seq_t)(sum / g);
}
