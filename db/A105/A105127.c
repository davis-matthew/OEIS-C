/*
 * A105127  Primes of the form 32n+17.
 * https://oeis.org/A105127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 17 modulo 32
 * Range:     u8 n <= 3, u16 n <= 403, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset starts at 1; value below offset is irrelevant */
    uint64_t count = 0;
    uint64_t cand = 17;            /* first integer of the form 32k+17 */
    while (1) {
        if (is_prime(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        cand += 32;
    }
}
