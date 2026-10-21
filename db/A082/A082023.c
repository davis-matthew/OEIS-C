/*
 * A082023  Number of partitions of n into 2 parts which are not relatively
 *          prime.
 * https://oeis.org/A082023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - φ(n))/2) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 689, u16 n <= 8406, u32 n <= 8406, u64 n <= 8406,
 *            u128 n <= 8406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Compute φ(n) with trial division. */
    uint64_t num = (uint64_t)n;
    uint64_t phi = num;
    uint64_t m   = num;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) {
                m /= p;
            }
            phi -= phi / p;
        }
    }
    if (m > 1) {
        phi -= phi / m;
    }

    /* a(n) = floor((n - φ(n)) / 2) */
    uint64_t diff = num - phi;
    return (seq_t)(diff / 2);
}
