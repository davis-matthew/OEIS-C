/*
 * A102853  Number of prime factors (with multiplicity) of number of points on
 *          surface of square pyramid.
 * https://oeis.org/A102853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001222(3*n^2 + 2) (number of prime factors with
 *            multiplicity)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = 3*n^2 + 2 using 128‑bit arithmetic to avoid overflow for idx_t≤64bit */
    unsigned __int128 m128 = (unsigned __int128)n * n;   /* n^2   */
    m128 = m128 * 3 + 2;                                 /* 3*n^2+2 */

    /* For the size of numbers used in this sequence (n up to a few thousand) 
       the value fits into 64 bits.  Convert to uint64_t for fast trial division. */
    uint64_t m = (uint64_t)m128;
    uint64_t temp = m;
    seq_t count = 0;

    /* Factor out 2 */
    while ((temp & 1ULL) == 0) {          /* temp % 2 == 0   */
        temp >>= 1;                       /* temp /= 2       */
        count++;
    }

    /* Factor odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        while ((temp % p) == 0) {
            temp /= p;
            count++;
        }
    }

    /* If anything remains it is a prime factor */
    if (temp > 1)
        count++;

    return count;
}
