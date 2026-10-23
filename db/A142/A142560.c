/*
 * A142560  Primes congruent to 30 mod 53.
 * https://oeis.org/A142560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 30 mod 53 (OEIS A142560) */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {   /* sequence is defined for n >= 1, but return 0 for safety */
        return 0;
    }

    idx_t count = 0;
    idx_t cand  = 83;   /* first prime ≡ 30 (mod 53) */

    while (1) {
        if (is_prime(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        cand += 53;    /* stay within the congruence class */
    }
}
