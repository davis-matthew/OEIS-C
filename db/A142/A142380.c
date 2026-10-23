/*
 * A142380  Primes congruent to 29 mod 47.
 * https://oeis.org/A142380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 29 (mod 47)
 * Range:     u8 n <= 1, u16 n <= 146, u32 n <= 147, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; n==0 not used in tests */

    seq_t candidate = 29;   /* first prime in the progression */
    idx_t count = 0;

    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 47;    /* step to the next number ≡29 (mod 47) */
    }
}
