/*
 * A142206  Primes congruent to 9 mod 41.
 * https://oeis.org/A142206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p mod 41 == 9
 * Range:     u8 n <= 1, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 – unused for n==0 */

    idx_t candidate = 9;           /* start from the first number with 9 mod 41 */
    idx_t count = 0;

    while (1) {
        candidate += 41;           /* walk through numbers congruent to 9 mod 41 */

        /* skip even candidates – only odd numbers can be prime (except 2) */
        if ((candidate & 1) == 0) continue;

        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
    }
}
