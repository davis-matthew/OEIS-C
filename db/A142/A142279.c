/*
 * A142279  Primes congruent to 30 mod 43.
 * https://oeis.org/A142279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime ≡ 30 (mod 43)
 * Range:     u8 n <= 1, u16 n <= 154, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* n ≥ 1 in the OEIS offset */
    idx_t count = 0;
    for (idx_t i = 0;; ++i) {
        idx_t candidate = 30 + ((idx_t)43 * i);
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
