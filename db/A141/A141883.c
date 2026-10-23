/*
 * A141883  Primes congruent to 9 mod 20.
 * https://oeis.org/A141883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 9 modulo 20
 * Range:     u8 n <= 5, u16 n <= 803, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: linearly scan candidates of the form 20*k + 9 and use the
 * provided is_prime() helper to test primality. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – defensive */
    idx_t count = 0;
    idx_t k = 0;                     /* start from k = 0  → candidate 9 */
    while (count < n) {
        idx_t cand = 20 * k + 9;
        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;  /* cast to output type */
        }
        ++k;
    }
    return 0; /* unreachable for n >= 1 */
}
