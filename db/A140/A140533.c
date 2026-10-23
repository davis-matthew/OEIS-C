/*
 * A140533  Primes congruent to 13 or 17 mod 30.
 * https://oeis.org/A140533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 13 or 17 modulo 30
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t cand = 13; ; ++cand) {
        if (cand % 30 == 13 || cand % 30 == 17) {
            if (is_prime(cand)) {
                ++count;
                if (count == n) {
                    return (seq_t)cand;
                }
            }
        }
    }
}
