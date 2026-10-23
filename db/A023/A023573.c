/*
 * A023573  Sum of distinct prime divisors of prime(n)-3.
 * https://oeis.org/A023573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime divisors of prime(n)-3
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   _smallest_prime_factor, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 guard */

    int64_t diff = (int64_t)get_prime(n) - 3;
    if (diff <= 1) return 0;        /* prime(n)-3 <= 1 */

    uint64_t m = (uint64_t)diff;
    seq_t sum = 0;

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        sum += (seq_t)p;
        while (m % p == 0) {        /* divide out this prime completely */
            m /= p;
        }
    }
    return sum;
}
