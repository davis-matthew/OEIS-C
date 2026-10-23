/*
 * A142614  Primes congruent to 18 mod 55.
 * https://oeis.org/A142614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 18 (mod 55), a(1)=73
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                       /* undefined for n < 1 */
    }

    idx_t found = 0;                     /* count of primes found so far */
    idx_t k = 1;                          /* index in the arithmetic progression 55*k+18 */

    while (1) {
        seq_t candidate = (seq_t)55 * k + 18;   /* generate numbers 55k + 18 */
        if (is_prime_big(candidate)) {          /* primality test */
            ++found;
            if (found == n) {
                return candidate;              /* desired term found */
            }
        }
        ++k;
    }
}
