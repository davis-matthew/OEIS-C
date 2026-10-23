/*
 * A028906  Arrange digits of primes in descending order.
 * https://oeis.org/A028906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrange the decimal digits of the nth prime in descending
 *            order
 * Range:     u8 n <= 26, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a(n) = A004186(A000040(n)) */

/* The function uses the provided helper get_prime(idx_t k) to obtain the nth prime,
 * then counts its decimal digits and rebuilds the integer with the digits sorted
 * from 9 down to 0. The result is returned in the generic integer type seq_t. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1-based. get_prime expects 1-based index. */
    uint32_t prime = get_prime(n);           /* nth prime, fits in 32 bits for n <= 10000 */
    seq_t result = 0;

    /* Count digits of prime */
    int cnt[10] = {0};
    uint32_t tmp = prime;
    if (tmp == 0)          /* prime cannot be 0, but handle just in case */
        cnt[0] = 1;
    while (tmp > 0) {
        int d = tmp % 10;
        ++cnt[d];
        tmp /= 10;
    }

    /* Reconstruct number with digits sorted descending */
    for (int d = 9; d >= 0; --d) {
        int c = cnt[d];
        while (c-- > 0)
            result = result * 10 + d;
    }

    return result;
}
