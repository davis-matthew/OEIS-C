/*
 * A074719  ip(n): the number of primes not exceeding reverse(n).
 * https://oeis.org/A074719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   a(n) = pi(reverse(n)) (reverse of n in base 10)
 * Range:     u8 n <= 1001, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Reverse the decimal representation of n */
    idx_t rev = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    /* Return the number of primes ≤ reverse(n) */
    return compute_pi(rev);
}
