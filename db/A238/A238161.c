/*
 * A238161  Greatest common divisor of the prime factors of n, each increased
 *          by 1.
 * https://oeis.org/A238161
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd_{p|n} (p + 1) (p runs over the distinct prime factors
 *            of n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, values below 2 are undefined */

    uint64_t m = (uint64_t)n;     /* work with native 64‑bit type for factorisation */
    seq_t g = 0;                  /* current GCD, 0 means not yet set */

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        g = (seq_t)2 + 1;                     /* 2 + 1 = 3 */
        while ((m & 1ULL) == 0) m >>= 1;
    }

    /* factor odd numbers */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t val = (seq_t)p + 1;
            if (g == 0) g = val;
            else g = gcd(g, val);
            while (m % p == 0) m /= p;
        }
    }

    /* remaining factor > 1 is prime */
    if (m > 1) {
        seq_t val = (seq_t)m + 1;
        if (g == 0) g = val;
        else g = gcd(g, val);
    }

    return g;
}
