/*
 * A213634  n-[n/m], where m is the least nondivisor of n (as in A007978) and
 *          [ ] = floor.
 * https://oeis.org/A213634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(n/m), where m is the least non‑divisor of n
 * Range:     u8 n <= 299, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* odd n -> (n+1)/2 = ceil(n/2) */
    if ((n & 1u) == 1u) {
        return (seq_t)((n >> 1) + (n & 1u));
    }

    /* find the least m ≥ 2 that does not divide n */
    idx_t m = 2;
    while (m <= n && (n % m) == 0) {
        ++m;
    }

    /* compute n - floor(n/m) */
    return (seq_t)n - (seq_t)(n / m);
}
