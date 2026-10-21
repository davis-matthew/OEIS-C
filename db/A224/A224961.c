/*
 * A224961  a(n) = number of primes of the form p * q + 2 where p is the
 *          prime(n) and q is any prime < p.
 * https://oeis.org/A224961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form p * q + 2 * where p =
 *            prime(n) and q is any prime < p. * The function uses the utility
 *            functions get_prime and is_prime_big. * It works for any seq_t
 *            type supported by the build
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 57, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For n = 0 we return 0; the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* Prime of index n.   get_prime returns a uint32_t.          */
    uint32_t p = get_prime(n);

    /* Count primes of the form p*q + 2 with q < p.             */
    seq_t cnt = 0;
    for (idx_t k = 1; k < n; ++k) {
        uint32_t q = get_prime(k);
        seq_t candidate = (seq_t)p * (seq_t)q + 2;
        if (is_prime_big(candidate))
            ++cnt;
    }
    return cnt;
}
