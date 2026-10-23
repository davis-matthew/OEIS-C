/*
 * A194113  a(n) = Sum_{j=1..n} floor(j*sqrt(10)); n-th partial sum of Beatty
 *          sequence for sqrt(10).
 * https://oeis.org/A194113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} floor(j*sqrt(10))
 * Range:     u8 n <= 12, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 : floor(sqrt(x)) */
static inline unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long low = 0, high = (unsigned long long)-1;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x) return mid;
        if (sq < x) low = mid + 1;
        else high = mid - 1;
    }
    return high;   /* floor of the square root */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        unsigned __int128 t = (unsigned __int128)10 *
                             (unsigned __int128)j *
                             (unsigned __int128)j;   /* 10 * j^2 */
        unsigned long long term = isqrt128(t);          /* floor(j*sqrt(10)) */
        sum += (seq_t)term;
    }
    return sum;
}
