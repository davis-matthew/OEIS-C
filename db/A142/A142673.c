/*
 * A142673  Primes congruent to 13 mod 57.
 * https://oeis.org/A142673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 13 modulo 57
 * Range:     u8 n <= 3, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;         /* sequence offset starts at 1 */
    uint64_t candidate = 13ULL;          /* first prime in the class */
    idx_t count = 0;

    while (1) {
        if (is_prime((idx_t)candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 57ULL;               /* keep the same congruence class */
    }
}
