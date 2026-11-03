/*
 * A051726  Numerator of n*(n-1)*(n-2)/720.
 * https://oeis.org/A051726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of n*(n-1)*(n-2)/720
 * Range:     u8 n <= 9, u16 n <= 78, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1865 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for __int128 values, result fits in seq_t (≤720) */
static seq_t gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

seq_t a(idx_t n)
{
    if (n < 3)                 /* product contains a zero factor  */
        return 0;

    unsigned __int128 N = (unsigned __int128)n * (n - 1) * (n - 2);

    seq_t g = gcd128(N, (unsigned __int128)720);
    return (seq_t)(N / g);
}
