/*
 * A167300  Totally multiplicative sequence with a(p) = 8*(p-2) for prime p.
 * https://oeis.org/A167300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative, a(p^e) = (8*(p-2))^e; a(2k)=0
 * Range:     u8 n <= 20, u16 n <= 374, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n = 1 => 1 by definition */
    if (n == 1) {
        return 1;
    }
    /* Prime 2 zeroes all even numbers */
    if ((n & 1) == 0) {
        return 0;
    }

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        /* 8*(p-2) = 8*p - 16 */
        seq_t base = (seq_t)8 * (seq_t)p - (seq_t)16;
        seq_t pow_val = big_pow(base, (seq_t)exp);
        result *= pow_val;
    }

    return result;
}
