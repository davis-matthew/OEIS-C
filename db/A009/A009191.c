/*
 * A009191  a(n) = gcd(n, d(n)), where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A009191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, d(n)) where d(n) is the number of divisors of n
 * Range:     u8 n <= 1835, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Count the integer divisors of n (n ≥ 1). */
static idx_t divisor_count(idx_t n)
{
    idx_t count = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (m > 1) {
        count *= 2;          /* remaining prime factor */
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard against n=0 */
    idx_t dcnt = divisor_count(n);
    seq_t d = (seq_t)dcnt;
    seq_t nn = (seq_t)n;
    return gcd(nn, d);
}
