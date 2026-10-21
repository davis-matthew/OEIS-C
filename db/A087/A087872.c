/*
 * A087872  Primes of the form (4*k + 3)^2 + (4*k + 2)^2 where k=0,1,2,3,...
 * https://oeis.org/A087872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form (4k+3)^2 + (4k+2)^2 = 32*k^2 +
 *            40*k + 13, k ≥ 0. The function searches increasing k, testing
 *            each candidate for primality with a deterministic Miller–Rabin
 *            test for 64‑bit integers
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 1607, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_u64, mulmod_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_u64.h"
#include "../utils/powmod_u64.h"

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* For n == 0 (before the offset) we return 0.  The offset is 1. */
    if (n == 0) return (seq_t)0;

    /* Static cache of found primes and the next k to test. */
    static seq_t *primes = NULL;
    static size_t size   = 0;   /* number of primes stored */
    static size_t cap    = 0;   /* capacity of the array */
    static idx_t  last_k = 0;   /* next k to examine */

    /* initialization on first call */
    if (primes == NULL)
    {
        cap = 256;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
        size   = 0;
    }

    /* generate primes until we have at least n of them */
    while (size < n)
    {
        seq_t k   = (seq_t)last_k;
        seq_t val = 32 * k * k + 40 * k + 13;   /* 32*k^2 + 40*k + 13 */

        /* test primality (cast to uint64_t – the values fit for n≤10000) */
        if (is_prime_u64((uint64_t)val))
        {
            if (size + 1 > cap)
            {
                cap *= 2;
                primes = (seq_t *)realloc(primes, cap * sizeof(seq_t));
            }
            primes[size++] = val;
        }
        ++last_k;
    }

    return primes[n - 1];
}
