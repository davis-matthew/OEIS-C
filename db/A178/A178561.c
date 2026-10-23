/*
 * A178561  Primes q from A178548.
 * https://oeis.org/A178561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4824, u32 n <= 4874, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, get_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A178561 (Primes q from A178548).  
   For n ≥ 1: let r = prime(n+2).  
   Find the smallest prime p such that q = 2·p + r is prime and  
   digit_sum(p) == digit_sum(q).  Return q. */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Out‑of‑range indices return 0. */
    if (n == 0) return 0;

    /* r = prime(n+2) */
    seq_t r = get_prime(n + 2);

    /* Search for the smallest prime p satisfying the condition. */
    uint64_t p = 2;                          /* first prime to test */
    for (;;) {
        /* q = 2 * p + r  (all arithmetic done in seq_t) */
        seq_t q = (seq_t)2 * (seq_t)p + r;

        /* Test the conditions. */
        if (is_prime_big(q) && digit_sum((seq_t)p) == digit_sum(q)) {
            return q;                      /* found the required q */
        }

        /* Advance p to the next prime. */
        p = next_prime(p + 1);
    }
}
