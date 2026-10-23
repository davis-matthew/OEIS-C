/*
 * A142650  Primes congruent to 23 mod 56.
 * https://oeis.org/A142650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 269, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142650: primes congruent to 23 mod 56 (incremental search) */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset is 1, undefined for n=0 */
    idx_t count = 0;
    idx_t candidate = 23;                /* first term of the progression */

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 56;                /* next number ≡ 23 (mod 56) */
    }
}
