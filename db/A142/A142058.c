/*
 * A142058  Primes congruent to 16 mod 33.
 * https://oeis.org/A142058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 16 modulo 33
 * Range:     u8 n <= 1, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the n-th prime p such that p % 33 == 16.
   Indexing starts at 1 (a(1) = 181). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1, but guard against 0 */
        return (seq_t)0;

    idx_t count = 0;
    idx_t cand  = 16;           /* start from 16; next candidates are cand += 33 */

    while (1) {
        cand += 33ULL;          /* next number congruent to 16 mod 33 */

        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
