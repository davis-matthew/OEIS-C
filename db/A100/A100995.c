/*
 * A100995  If n is a prime power p^m, m >= 1, then m, otherwise 0.
 * https://oeis.org/A100995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the prime factor of n if n is a prime power,
 *            otherwise 0
 * Range:     u8 n <= 9756, u16 n <= 9756, u32 n <= 9756, u64 n <= 9756,
 *            u128 n <= 9756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    idx_t temp = n;
    idx_t d;
    int exponent = 0;

    if ((temp & 1ULL) == 0)  /* n is even */
    {
        d = 2;
        while (temp % d == 0) {
            temp /= d;
            exponent++;
        }
    }
    else  /* n is odd */
    {
        d = 3;
        int found = 0;
        for (; d <= temp / d; d += 2) {
            if (temp % d == 0) {
                found = 1;
                break;
            }
        }
        if (found) {
            while (temp % d == 0) {
                temp /= d;
                exponent++;
            }
        } else {
            /* n is prime */
            return 1;
        }
    }

    return (temp == 1) ? (seq_t)exponent : 0;
}
