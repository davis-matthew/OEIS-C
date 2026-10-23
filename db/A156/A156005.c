/*
 * A156005  Primes p such that 16*p+45 is prime.
 * https://oeis.org/A156005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 16*p+45 is prime
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    idx_t p = 2;
    while (1) {
        if (is_prime(p)) {
            idx_t q = 16 * p + 45;
            if (is_prime(q)) {
                ++count;
                if (count == n) {
                    return (seq_t)p;
                }
            }
        }
        if (p <= 2) {
            p = 3;
        } else {
            p += 2;   /* skip even numbers */
        }
    }
}
