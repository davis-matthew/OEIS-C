/*
 * A059858  Primes p such that x^5 == 2 (mod p) has five solutions.
 * https://oeis.org/A059858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   modpow2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059858:
 *  Primes p such that x^5 == 2 (mod p) has five solutions.
 *  Equivalent to p ≡ 1 (mod 5) and 2^((p‑1)/5) ≡ 1 (mod p).
 *  a(n) is the nth such prime, offset 1.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/modpow2_uint64.h"

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Storage for the primes that satisfy the condition.                   */
static uint64_t *goodPrimes = (uint64_t *)0;
static size_t   goodCnt    = 0;
static int      initialized = 0;

/* --------------------------------------------------------------------- */
/* One‑time sieve and prime collection.  The limit is chosen to be
 * large enough for up to at least ten thousand terms. */
static void init_sequence(void)
{
    if (initialized)
        return;

    /* Sieve limit (10 million) yields over ten thousand good primes. */
    const size_t LIMIT = 10000000ULL;
    const size_t BITSET_SIZE = (LIMIT >> 3) + 1;

    unsigned char *comp = (unsigned char *)calloc(BITSET_SIZE, 1);
    if (!comp)
        exit(1);
    comp[0] |= 1; /* mark 0 as composite */
    comp[1] |= 1; /* mark 1 as composite */

    for (size_t i = 2; i * i <= LIMIT; ++i)
    {
        if (!(comp[i >> 3] & (1 << (i & 7))))
        {
            for (size_t j = i * i; j <= LIMIT; j += i)
                comp[j >> 3] |= 1 << (j & 7);
        }
    }

    /* Rough upper bound on the number of good primes. */
    const size_t EST_GUESS = LIMIT / 20 + 100;
    goodPrimes = (uint64_t *)malloc(EST_GUESS * sizeof(uint64_t));
    if (!goodPrimes)
        exit(1);
    goodCnt = 0;

    for (uint64_t p = 2; p <= LIMIT; ++p)
    {
        if (!(comp[p >> 3] & (1 << (p & 7)))) /* p is prime          */
        {
            if ((p % 5) == 1)
            {
                seq_t val = modpow2_uint64((p - 1) / 5, p);
                if (val == 1)
                    goodPrimes[goodCnt++] = p;
            }
        }
    }

    free(comp);
    initialized = 1;
}

/* --------------------------------------------------------------------- */
/* Requested sequence value.  n is 1‑based; n == 0 is unused.           */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    init_sequence();
    if ((size_t)n > goodCnt)
        return 0; /* Guard; should not occur for valid input  */
    return (seq_t)goodPrimes[n - 1];
}
