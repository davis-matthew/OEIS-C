/*
 * A170820  Let p = n-th prime; a(n) = (p-1)/(order of (p+3)/2 mod p).
 * https://oeis.org/A170820
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6382, u32 n <= 7297, u64 n <= 9271,
 *            u128 n <= 9271, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A170820: a(n) = (p-1)/(order of ((p+3)/2) mod p)
   where p is the n-th prime.  Offset 3.  Uses the helper
   get_prime(idx_t k) to obtain the n-th prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Modulo exponentiation:  base^exp (mod mod)  */
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = (b * result) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)               /* values below the offset are undefined */
        return 0;

    /* obtain the n-th prime; n fits into idx_t and the prime into 32‑bit */
    uint32_t p = get_prime(n);

    /* work with seq_t types for the calculation */
    seq_t base = ((seq_t)p + 3) / 2;   /* (p+3)/2 */
    seq_t mod  = p;                    /* mod p */
    seq_t order = mod - 1;             /* start with p-1 */

    /* factor p-1 by trial division; p-1 <= 104728 for n≤10 000 */
    uint64_t rem = (uint64_t)(mod - 1);
    for (uint64_t i = 2; i * i <= rem; ++i)
    {
        if (rem % i == 0)
        {
            /* i is a prime factor of (p-1) */
            while (rem % i == 0)      /* remove all copies of i */
                rem /= i;

            seq_t q = (seq_t)i;
            while (order % q == 0 &&
                   mod_pow(base, (uint64_t)(order / q), mod) == 1)
                order /= q;
        }
    }
    if (rem > 1)                 /* remaining prime factor > sqrt(p-1) */
    {
        seq_t q = (seq_t)rem;
        while (order % q == 0 &&
               mod_pow(base, (uint64_t)(order / q), mod) == 1)
            order /= q;
    }

    return ((seq_t)p - 1) / order;
}
