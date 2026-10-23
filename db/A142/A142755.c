/*
 * A142755  Primes congruent to 28 mod 59.
 * https://oeis.org/A142755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes congruent to 28 mod 59
 * Range:     u8 none, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* n is 1‑based. Find the nth prime p with p % 59 == 28. */
    idx_t cand = 28;          /* first candidate */
    idx_t count = 0;          /* how many such primes found */

    while (1) {
        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 59;
    }
}
