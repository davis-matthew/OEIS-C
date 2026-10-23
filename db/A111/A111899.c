/*
 * A111899  Number of numbers m <= n such that 9 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers m ≤ n whose square root has * 9 as the
 *            second digit after the decimal point. * The second decimal digit
 *            of sqrt(m) is the last digit of * floor( sqrt(m) * 100 ). Since
 *            sqrt(m) * 100 = sqrt( m * 10000 ), * we can compute the digit by
 *            taking the integer square root of * m...
 * Range:     u8 n <= 2589, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
 * Returns floor(sqrt(x)).  Works for all 0 ≤ x < 2^128. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1; /* max sqrt(2^128-1) */
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

/* Main sequence function.  seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned __int128 M = (unsigned __int128)i * 10000u; /* i * 10000 */
        unsigned __int128 t = isqrt128(M);                 /* floor(sqrt(i*10000)) */
        if ((unsigned int)(t % 10) == 9)
            ++count;
    }
    return count;
}
