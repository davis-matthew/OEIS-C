/*
 * A095402  Sum of digits of all distinct prime factors of n.
 * https://oeis.org/A095402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the decimal digit sums of the distinct prime
 *            factors of n (a(1)=0). The implementation uses trial division
 *            with a local digit‑sum routine that works for all supported
 *            seq_t types
 * Range:     u8 n <= 400, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard integer type definitions */
#include <stdint.h>
#include "../utils/_digit_sum.h"

/* Required entry point */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                 /* a(1)=0 by definition */

    uint64_t m = (uint64_t)n;            /* n fits in uint64_t */
    seq_t sum = 0;

    /* Handle factor 2 separately to keep the loop odd */
    if ((m & 1ULL) == 0) {
        sum += _digit_sum((seq_t)2);
        while ((m & 1ULL) == 0)
            m >>= 1;                     /* divide by 2 */
    }

    /* Trial division by odd numbers up to sqrt(m) */
    for (uint64_t f = 3; f * f <= m; f += 2) {
        if (m % f == 0) {
            sum += _digit_sum((seq_t)f);
            while (m % f == 0)
                m /= f;
        }
    }

    /* If remainder > 1, it's a prime factor > sqrt(original n) */
    if (m > 1)
        sum += _digit_sum((seq_t)m);

    return sum;
}
