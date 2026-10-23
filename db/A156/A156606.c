/*
 * A156606  a(n)=number of even digits in prime(n) + number of prime digits in
 *          prime(n).
 * https://oeis.org/A156606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = (number of even digits in prime(n)) + (number of prime
 *            digits in prime(n)) * where prime(n) is the n-th prime
 *            (prime(1)=2)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) → uint32_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; behavior for 0 is irrelevant */

    uint32_t p = get_prime(n);   /* retrieve the n-th prime, fits in uint32_t */

    unsigned int even_cnt = 0;
    unsigned int prime_digit_cnt = 0;

    /* Count digits of the prime */
    while (p > 0) {
        unsigned int d = p % 10;
        if (d % 2 == 0)           /* even digits: 2,4,6,8 */
            ++even_cnt;

        /* prime digits: 2,3,5,7 */
        if (d == 2 || d == 3 || d == 5 || d == 7)
            ++prime_digit_cnt;

        p /= 10;
    }

    return (seq_t)even_cnt + (seq_t)prime_digit_cnt;
}
