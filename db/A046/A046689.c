/*
 * A046689  Primes p such that p+1 divides 24.
 * https://oeis.org/A046689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that p+1 divides 24; finite list computed by a small sieve. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    static seq_t primes[6];
    static bool initialized = false;

    if (!initialized) {
        const uint16_t divisors[8] = {1, 2, 3, 4, 6, 8, 12, 24};
        size_t cnt = 0;
        for (size_t i = 0; i < 8; ++i) {
            seq_t p = (seq_t)divisors[i] - (seq_t)1;
            if (p >= (seq_t)2 && is_prime_small(p)) {
                primes[cnt++] = p;
            }
        }
        initialized = true;
    }

    if (n == 0) return (seq_t)0;          /* offset 1; guard against n==0 */
    if (n <= 6) return primes[n - 1];     /* return the n‑th term */
    return (seq_t)0;                      /* no more terms in the sequence */
}
