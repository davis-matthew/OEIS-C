/*
 * A091306  Sum of squares of unitary, squarefree divisors of n, including 1.
 * https://oeis.org/A091306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (p²+1) if e=1, else 1; multiplicative
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not used, but safe for n<1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++exp; }
        if (exp == 1) {
            seq_t p = (seq_t)2;
            result *= (p * p + (seq_t)1);
        }
    }

    /* odd factors */
    for (uint64_t d = 3; d <= m/d; d += 2) {
        if (m % d == 0) {
            uint64_t exp = 0;
            while (m % d == 0) { m /= d; ++exp; }
            if (exp == 1) {
                seq_t p = (seq_t)d;
                result *= (p * p + (seq_t)1);
            }
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        seq_t p = (seq_t)m;
        result *= (p * p + (seq_t)1);
    }

    return result;
}
