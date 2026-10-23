/*
 * A129354  Duplicate of A129628.
 * https://oeis.org/A129354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = T_{k+1} * d(m) * where n = 2^k * m (m odd), T_{k+1} =
 *            (k+1)(k+2)/2 (triangular number), * and d(m) is the number of
 *            divisors of m
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard, offset is 1 */

    /* separate power of two */
    uint64_t k = 0;
    uint64_t temp = n;
    while ((temp & 1ULL) == 0) {   /* count exponent of 2 */
        ++k;
        temp >>= 1;
    }
    uint64_t m = temp;             /* odd part of n */

    /* divisor count of odd part m */
    uint64_t d = 1;
    uint64_t x = m;
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            d *= (e + 1);
        }
    }
    if (x > 1) d *= 2;             /* remaining prime factor */

    /* triangular number T_{k+1} */
    seq_t T = (seq_t)(k + 1) * (seq_t)(k + 2) / 2;

    return T * (seq_t)d;
}
