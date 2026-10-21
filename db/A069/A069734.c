/*
 * A069734  Number of pairs (p,q), 0<=p<=q, such that p+q divides n.
 * https://oeis.org/A069734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} (1 + floor(d/2))
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of pairs (p,q), 0 ≤ p ≤ q, with p+q dividing n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handles n ≥ 0; for n = 0 return 0 (unused by OEIS). */
    seq_t result = 0;
    if (n == 0) return result;

    /* Enumerate all divisors d of n. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            result += (seq_t)(1 + (i >> 1));
            /* divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != i) {
                result += (seq_t)(1 + (d2 >> 1));
            }
        }
    }
    return result;
}
