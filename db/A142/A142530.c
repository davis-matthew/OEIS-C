/*
 * A142530  Primes congruent to 49 mod 52.
 * https://oeis.org/A142530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 253, u32 n <= 260, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 49 mod 52 (search) */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* 1‑based sequence, 0 is invalid */

    idx_t count = 0;              /* how many qualifying primes seen */
    seq_t candidate = (seq_t)101; /* first prime ≡ 49 (mod 52) */

    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        candidate = candidate + 52; /* next number with residue 49 modulo 52 */
    }
}
