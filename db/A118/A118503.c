/*
 * A118503  Sum of digits of prime factors of n, with multiplicity.
 * https://oeis.org/A118503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 672, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118503: sum of the digit sums of the prime factors of n (with multiplicity) */

#include <stdint.h>
#include "../utils/digit_sum.h"   /* declares seq_t digit_sum(seq_t v) */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 0 */
        return 0;

    uint64_t x = (uint64_t)n;
    seq_t sum = 0;

    /* factor 2 separately */
    while ((x & 1ULL) == 0) {                 /* while x is even */
        sum += digit_sum((seq_t)2);
        x >>= 1;                              /* x /= 2 */
    }

    /* odd factors up to sqrt(x) */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        while (x % p == 0) {
            sum += digit_sum((seq_t)p);
            x /= p;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (x > 1)
        sum += digit_sum((seq_t)x);

    return sum;
}
