/*
 * A142043  Primes congruent to 21 mod 32.
 * https://oeis.org/A142043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p ≡ 21 (mod 32) – searched sequentially
 * Range:     u8 n <= 3, u16 n <= 422, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 959 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard anyway */
    const seq_t start = 53;        /* first prime ≡ 21 (mod 32) */
    seq_t candidate = start;
    idx_t count = 0;

    while (true) {
        if (is_prime_seq(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 32;           /* keep the congruence class 21 (mod 32) */
    }
}
