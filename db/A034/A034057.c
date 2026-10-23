/*
 * A034057  Decimal part of a(n)^(1/2) starts with n (squares excluded).
 * https://oeis.org/A034057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = the smallest integer k, not a perfect square, such that *
 *            the decimal part of sqrt(k) starts with the decimal
 *            representation of n
 * Range:     u8 n <= 24, u16 n <= 499, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute the number of decimal digits of an unsigned integer. */
static int num_digits(idx_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x) { x /= 10; ++d; }
    return d;
}

seq_t a(idx_t n)
{
    /* Determine the number of digits of n and 10^d, 10^(2d). */
    int d = num_digits(n);
    __int128 pow10d = 1;
    for (int i = 0; i < d; ++i) pow10d *= 10;
    __int128 pow10d2 = pow10d * pow10d;

    for (idx_t m = 0; ; ++m) {
        __int128 mm = m;
        /* Lower bound of k derived from the interval for sqrt(k). */
        __int128 base = (__int128)n + pow10d * mm;
        __int128 num  = base * base;
        __int128 L = (num + pow10d2 - 1) / pow10d2;          /* ceil */

        __int128 mm2 = mm * mm;
        if (L == mm2) ++L;             /* skip perfect squares */

        /* Upper bound of k corresponding to the same interval. */
        __int128 baseU = (__int128)n + 1 + pow10d * mm;
        __int128 numU  = baseU * baseU;
        __int128 upper = (numU - 1) / pow10d2;          /* floor */

        if (L > upper) continue;         /* no integer k in this range */

        /* Ensure k is still less than (m+1)^2. */
        __int128 maxInSquare = (mm + 1) * (mm + 1) - 1;
        if (L > maxInSquare) continue;

        return (seq_t)L;
    }
}
