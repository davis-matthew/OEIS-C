/*
 * A140559  Primes p such that p + 34 is not a prime.
 * https://oeis.org/A140559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime p for which (p + 34) is not prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* collection is 1‑based; 0 is unused */

    idx_t counted = 0;
    uint64_t p = 2;                 /* start with the first prime */

    while (counted < n) {
        if (!is_prime_small((seq_t)(p + 34))) {
            ++counted;
            if (counted == n) {
                return (seq_t)p;
            }
        }
        p = next_prime(p + 1);      /* advance to the next prime */
    }

    /* The loop always terminates for n ≥ 1, so this point is never reached. */
    return 0;
}
