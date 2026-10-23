/*
 * A036121  5^n mod 23.
 * https://oeis.org/A036121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n;   /* copy of n */
    unsigned long long r = 1ULL;   /* result */
    unsigned long long b = 5ULL % 23ULL; /* base 5 mod 23 */

    while (m)
    {
        if (m & 1ULL)            /* if lowest bit set */
            r = (r * b) % 23ULL; /* multiply then reduce modulo 23 */
        b = (b * b) % 23ULL;    /* square and reduce */
        m >>= 1ULL;             /* move to next bit */
    }
    return (seq_t)r;
}
