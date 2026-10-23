/*
 * A138754  a(n) = PrimePi(A138751(n)) - a variation of the Collatz (3n+1)
 *          map.
 * https://oeis.org/A138754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime_pi(next_prime(update_rule(prime(n))))
 * Range:     u8 n <= 33, u16 n <= 4148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where update_rule(p) = 2*p if p == 3 or p ≡ 1 (mod 3),
 *                      otherwise floor(p/2).                     */

#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k);          */
#include "../utils/next_prime.h"  /* uint64_t next_prime(uint64_t start); */
#include "../utils/prime_pi.h"    /* seq_t prime_pi(idx_t limit);         */

seq_t a(idx_t n)
{
    /* 1. nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* 2. Apply the Collatz‑style rule. */
    seq_t m1;
    if (p == 3 || (p % 3) == 1)
        m1 = (seq_t)p * 2;          /* double */
    else
        m1 = (seq_t)p / 2;          /* floor division */

    /* 3. Next prime > m1.  next_prime expects a 64‑bit start value. */
    uint64_t next = next_prime((uint64_t)m1);

    /* 4. Number of primes ≤ next – the required sequence value. */
    return prime_pi(next);
}
