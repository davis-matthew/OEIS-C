/*
 * A106611  a(n) = numerator of n/(n+10).
 * https://oeis.org/A106611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n / gcd(n, n+10)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The numerator of the fraction n/(n+10) */
#include <stdint.h>

static idx_t my_gcd(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t g = my_gcd(n, n + (idx_t)10);
    return (seq_t)(n / g);
}
