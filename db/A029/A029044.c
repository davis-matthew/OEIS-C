/*
 * A029044  Expansion of 1/((1-x)*(1-x^3)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A029044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 63*n^2 + 552*n + 60*n*[(n mod 3)=0] +
 *            2295)/2160) + [(n mod 60)=30]
 * Range:     u8 n <= 54, u16 n <= 403, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit intermediate to avoid overflow for normal input ranges. */
    unsigned long long tn = n;
    unsigned long long n2 = tn * tn;
    unsigned long long n3 = n2 * tn;

    unsigned long long mult30 = (n % 3 == 0) ? 1ULL : 0ULL;
    unsigned long long num = 2ULL * n3
                           + 63ULL * n2
                           + 552ULL * tn
                           + 60ULL * tn * mult30
                           + 2295ULL;

    unsigned long long res = num / 2160ULL;
    if (n % 60 == 30)
        res += 1ULL;

    return (seq_t)res;
}
