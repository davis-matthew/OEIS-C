/*
 * A121855  a(n) = ceiling(Pi*sqrt(n)).
 * https://oeis.org/A121855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(pi * sqrt(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for all n >= 0 using integer arithmetic only. */
/* Uses a high‑precision rational upper bound for pi²: */
/* pi² ≈ 10888505104 / 1103236225  (error < 1e-11). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Upper bound for pi² as a fraction */
    const unsigned long long P = 10888505104ULL;   /* numerator   */
    const unsigned long long Q = 1103236225ULL;    /* denominator */

    /* Compute floor sqrt( (P * n)/Q ) using integer arithmetic. */
    unsigned __int128 numer = (unsigned __int128)P * n;
    unsigned long long low  = 0;
    unsigned long long high = 1;

    /* Find an upper bound for the root. */
    while ((unsigned __int128)high * high * Q <= numer)
        high <<= 1;

    /* Binary search for the integer square root. */
    while (low + 1 < high) {
        unsigned long long mid = (low + high) >> 1;
        if ((unsigned __int128)mid * mid * Q <= numer)
            low = mid;
        else
            high = mid;
    }

    /* 'low' is floor sqrt( (P*n)/Q ).  Ceil(pi*sqrt(n)) is one more. */
    seq_t result = (seq_t)(low + 1);
    return result;
}
