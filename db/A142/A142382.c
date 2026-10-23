/*
 * A142382  Primes congruent to 31 mod 47.
 * https://oeis.org/A142382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 31 mod 47
 * Range:     u8 n <= 1, u16 n <= 141, u32 n <= 142, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence starts at index 1 */

    seq_t cand = (seq_t)31;    /* first number ≡ 31 (mod 47) */
    idx_t count = 0;

    while (1) {
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + (seq_t)47;   /* next candidate with residue 31 mod 47 */
    }
}
