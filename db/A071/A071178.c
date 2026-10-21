/*
 * A071178  Exponent of the largest prime factor of n.
 * https://oeis.org/A071178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = exponent of the largest prime factor of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t temp = n;
    uint64_t largest = 0;
    seq_t exp = 0;

    /* factor 2 */
    if (temp % 2 == 0) {
        uint64_t cnt = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            cnt++;
        }
        largest = 2;
        exp = cnt;
    }

    /* factor odd primes */
    for (uint64_t f = 3; f <= temp / f; f += 2) {
        if (temp % f == 0) {
            uint64_t cnt = 0;
            while (temp % f == 0) {
                temp /= f;
                cnt++;
            }
            if (f > largest) {
                largest = f;
                exp = cnt;
            }
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        if (temp > largest) {
            largest = temp;
            exp = 1;
        }
    }

    return exp;
}
