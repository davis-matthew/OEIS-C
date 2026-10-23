/*
 * A138191  Denominator of (n-1)*n*(n+1)/12.
 * https://oeis.org/A138191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult, easy
 * Formula:   a(n) = 12 / gcd((n-1)*n*(n+1),12)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* compute (n-1)*n*(n+1) modulo 12 */
    uint64_t r1 = (n-1) % 12;
    uint64_t r2 = n % 12;
    uint64_t r3 = (n+1) % 12;
    uint64_t prod = (r1 * r2) % 12;
    prod = (prod * r3) % 12;

    /* Euclidean algorithm to get gcd(prod,12) */
    uint64_t x = prod, y = 12;
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    uint64_t g = x;              /* gcd(prod,12) */

    return (seq_t)(12 / g);      /* denominator of (n-1)*n*(n+1)/12 */
}
