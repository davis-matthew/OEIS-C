/*
 * A141924  Primes congruent to 20 mod 23.
 * https://oeis.org/A141924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime ≡ 20 (mod 23)
 * Range:     u8 n <= 4, u16 n <= 298, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    idx_t count = 0;
    while (1) {
        seq_t candidate = (seq_t)23 * k + (seq_t)20;
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++k;
    }
}
