/*
 * A045385  Primes congruent to {2, 4, 6} mod 7.
 * https://oeis.org/A045385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime p with p ≡ {2,4,6} (mod 7) – the n‑th such prime */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Returns the nth prime p satisfying p % 7 ∈ {2,4,6} */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe return */

    idx_t count = 0;
    seq_t candidate = 2;

    for (;;) {
        if (is_prime_big(candidate)) {
            seq_t mod = candidate % (seq_t)7;
            if (mod == (seq_t)2 || mod == (seq_t)4 || mod == (seq_t)6) {
                count++;
                if (count == n) return candidate;
            }
        }
        candidate++;                 /* next integer to test */
    }
}
