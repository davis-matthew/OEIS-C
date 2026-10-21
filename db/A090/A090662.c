/*
 * A090662  Duplicate of A088387.
 * https://oeis.org/A090662
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 if n is a power of 2 or n is divisible by 12; otherwise
 *            a(n) = the largest odd prime divisor of n
 * Range:     i8 n <= 32, i16 n <= 32, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 2. */
    if (n < 2)
        return (seq_t)0; /* undefined for offset below 2 */

    /* Check for special cases returning 2.  Power of 2 is tested by
       the classic bit trick; divisibility by 12 is checked with a
       modulus. */
    if ( (n % 12ULL == 0) || ((n & (n - 1ULL)) == 0ULL) )
        return (seq_t)2;

    /* Remove all factors of 2.  After this step `tmp` is the odd part
       of n.  Since n was not a power of 2, this odd part is > 1. */
    uint64_t tmp = n;
    while ((tmp & 1ULL) == 0ULL)
        tmp >>= 1;

    /* Compute the largest odd prime factor of `tmp`. */
    uint64_t m = tmp;              /* odd part */
    uint64_t largest = 1ULL;

    /* Treat 3 separately to allow the 6k ± 1 loop. */
    if (m % 3ULL == 0ULL) {
        largest = 3ULL;
        while (m % 3ULL == 0ULL)
            m /= 3ULL;
    }

    for (uint64_t f = 5ULL; f <= m / f; f += 6ULL) {
        if (m % f == 0ULL) {
            largest = f;
            while (m % f == 0ULL)
                m /= f;
        }
        uint64_t f2 = f + 2ULL;    /* check the second of the 6k±1 pair */
        if (m % f2 == 0ULL) {
            largest = f2;
            while (m % f2 == 0ULL)
                m /= f2;
        }
    }

    /* Any remaining factor > 1 is a prime factor. */
    if (m > 1ULL)
        largest = m;

    return (seq_t)largest;
}
