/*
 * A141944  Primes congruent to 22 mod 25.
 * https://oeis.org/A141944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime congruent to 47 mod 50 (i.e., 22 mod 25)
 * Range:     u8 n <= 3, u16 n <= 279, u32 n <= 280, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t p = 47;                      /* first candidate */

    while (true) {
        if (is_prime_big(p)) {
            ++count;
            if (count == n)
                return p;
        }
        p += 50;                       /* next number ≡ 47 (mod 50) */
    }
}
