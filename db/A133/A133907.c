/*
 * A133907  Least prime number p such that binomial(n+p, p) mod p = 1.
 * https://oeis.org/A133907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A133907
 *  Formula:  a(n) = the least prime p such that
 *            binomial(n+p, p) ≡ 1 (mod p)
 *  Equivalent condition: p divides ⌊n/p⌋ or p > n.
 *
 *  This implementation tests primes sequentially using the provided
 *  helper function next_prime(uint64_t start) until the condition is
 *  satisfied.  It works for any n ≥ 1 and is fast for the ranges
 *  requested in the observations (up to at least 10 000).
 */

#include <stdint.h>
#include "../utils/next_prime.h"

/* Return the A133907 term for index n (1‑based). */
seq_t a(idx_t n)
{
    idx_t p = 2;                       /* start with the first prime */
    while (1)
    {
        if (p > n)                    /* p > n ⇒ condition automatically true */
            return (seq_t)p;

        idx_t k = n / p;              /* ⌊n/p⌋ */
        if ((k % p) == 0)             /* p divides ⌊n/p⌋ */
            return (seq_t)p;

        p = next_prime(p);            /* next prime after p */
    }
}
