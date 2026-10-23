/*
 * A142907  Primes congruent to 32 mod 63.
 * https://oeis.org/A142907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime ≡ 32 (mod 63)
 * Range:     u8 none, u16 n <= 148, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for indices below the offset */

    idx_t   count = 0;              /* how many primes found so far   */
    idx_t   k     = 5;              /* start from the first candidate 63*5+32 = 347 */
    seq_t   result = 0;

    while (count < n) {
        seq_t cand = (seq_t)63 * (seq_t)k + (seq_t)32; /* 63·k + 32            */
        ++k;
        if (is_prime_big(cand)) {
            ++count;
            result = cand;
        }
    }
    return result;
}
