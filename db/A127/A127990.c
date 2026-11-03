/*
 * A127990  Largest prime factor of 2*n^3 - 2*n + 9.
 * https://oeis.org/A127990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of 2*n^3 - 2*n + 9
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 1860, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9369 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    /* No meaningful value for n < 1; return 0 */
    if (n < 1) return 0;

    uint64_t x = (uint64_t)n;
    unsigned __int128 n128 = x;
    unsigned __int128 v128 = 2 * n128 * n128 * n128 - 2 * n128 + 9;
    uint64_t v = (uint64_t)v128;  /* value of 2*n^3 - 2*n + 9 */

    uint64_t lpf = largest_prime_divisor(v);
    return (seq_t)lpf;
}
