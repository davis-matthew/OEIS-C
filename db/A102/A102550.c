/*
 * A102550  Number of distinct prime-factors of n that are bitwise covered by
 *          n.
 * https://oeis.org/A102550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors p of n with (p & n) == p
 *            (i.e. primes that are bitwise covered by n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t count = 0;
    idx_t temp = n;

    /* handle factor 2 */
    if (~n & 1) {                      /* n is even */
        if ((n & 2) == 2)
            count++;
        while ((temp & 1) == 0)
            temp >>= 1;
    }

    /* try odd divisors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if ((n & p) == p)
                count++;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        if ((n & temp) == temp)
            count++;
    }

    return count;
}
