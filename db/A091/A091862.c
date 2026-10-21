/*
 * A091862  a(n) = 1 if the sum of all exponents of the prime-factorization of
 *          n has no carries when summed in base 2, or a(n) = 0 if there are
 *          any carries.
 * https://oeis.org/A091862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the bitwise OR of the exponents in the prime
 *            factorisation of n equals the sum of those exponents (no carries
 *            when adding them in base 2), 0 otherwise
 * Range:     u8 n <= 9444, u16 n <= 9444, u32 n <= 9444, u64 n <= 9444,
 *            u128 n <= 9444, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle n == 0 (outside offset) – simply return 0 */
    if (n == 0) return (seq_t)0;

    idx_t temp = n;
    idx_t sumE = 0;      /* sum of exponents */
    idx_t orE  = 0;      /* bitwise OR of all exponents */

    /* factor out 2 */
    if ((temp & 1ULL) == 0) {
        idx_t cnt = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++cnt;
        }
        sumE += cnt;
        orE  |= cnt;
    }

    /* factor odd divisors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            idx_t cnt = 0;
            while (temp % d == 0) {
                temp /= d;
                ++cnt;
            }
            sumE += cnt;
            orE  |= cnt;
        }
    }

    /* if remainder > 1, it's a prime factor with exponent 1 */
    if (temp > 1) {
        sumE += 1;
        orE  |= 1;
    }

    return (seq_t)(orE == sumE);
}
