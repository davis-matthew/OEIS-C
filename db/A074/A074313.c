/*
 * A074313  a(n) = the maximal length of a sequence of primes {s_1 = prime(n),
 *          s_2 = f(s1), s_3 = f(s_2), ....} formed by repeated application of
 *          f(m) = Floor(m/2) on prime(n).
 * https://oeis.org/A074313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A074313: a(n) = maximal length of a prime chain obtained by
   repeatedly applying f(m) = ⌊m/2⌋ to the n‑th prime.  The chain
   terminates when the value is no longer prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"       /* uint32_t get_prime(idx_t k) */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n) */

seq_t a(idx_t n)
{
    /* Get the n‑th prime (offset 1). */
    seq_t p = (seq_t)get_prime(n);
    seq_t len = 1; /* the prime itself counts */

    /* Iteratively halve and count primes until the value stops being prime. */
    while (p > 1) {
        p >>= 1;                     /* floor(p/2) */
        if (is_prime_big(p))
            ++len;
        else
            break;
    }
    return len;
}
