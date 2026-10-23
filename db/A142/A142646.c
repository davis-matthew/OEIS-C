/*
 * A142646  Primes congruent to 13 mod 56.
 * https://oeis.org/A142646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 13 (mod 56)
 * Range:     u8 n <= 2, u16 n <= 248, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 for safety */

    seq_t cand = 13;                /* first prime in the progression */
    idx_t  count = 1;               /* we already have the first prime */

    if (n == 1) return cand;

    /* keep stepping by 56 until we find the n‑th prime */
    while (count < n) {
        cand = cand + (seq_t)56;
        if (is_prime_big(cand))
            ++count;
    }
    return cand;
}
