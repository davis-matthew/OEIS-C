/*
 * A239683  Number of digits in decimal expansion of n^5.
 * https://oeis.org/A239683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n^5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the number of decimal digits of an unsigned __int128 value */
static inline unsigned long long u128_digit_len(unsigned __int128 x)
{
    unsigned long long len = 0;
    do {
        ++len;
        x /= 10;
    } while (x);
    return len;
}

seq_t a(idx_t n)
{
    unsigned __int128 r = (unsigned __int128)n;
    r *= r;                          /* n^2 */
    r *= (unsigned __int128)n;        /* n^3 */
    r *= (unsigned __int128)n;        /* n^4 */
    r *= (unsigned __int128)n;        /* n^5 */

    unsigned long long len = u128_digit_len(r);
    return (seq_t)len;
}
