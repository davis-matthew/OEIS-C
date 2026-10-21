/*
 * A095075  Primes in whose binary expansion the number of 1-bits is less than
 *          or equal to number of 0-bits.
 * https://oeis.org/A095075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   A095075 – primes whose binary expansion has ≤ half of the bits
 *            equal to 1 a(n) = the n‑th prime p such that 2*popcount(p) ≤
 *            bitlen(p), where bitlen(p) = floor(log₂(p)) + 1
 * Range:     u8 n <= 14, u16 n <= 2307, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Cache for the sequence values (a(1), a(2), …).  A few hundred thousand
   slots are more than enough for n up to 10 000.                          */
/* --------------------------------------------------------------------- */
#define MAX_CACHE 200000
static seq_t   cache[MAX_CACHE];
static size_t  cache_sz = 0;

/* --------------------------------------------------------------------- */
/*  Prime sieve data: we generate primes on the fly and store them in a
   simple array for trial division.                                       */
/* --------------------------------------------------------------------- */
#define MAX_PRIMES 200000
static uint64_t primes[MAX_PRIMES];
static size_t  primes_sz = 0;
static uint64_t next_candidate = 2;   /* integer to test for primality */

/* --------------------------------------------------------------------- */
/*  Trial division primality test.  Only small primes are needed up to
   the 10 000th qualifying prime (well below 2³²).                           */
/* --------------------------------------------------------------------- */
static inline bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_sz; ++i)
    {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* p*p > n, no need to continue */
        if (n % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Store a new prime discovered during the search.                        */
/* --------------------------------------------------------------------- */
static void add_prime(uint64_t p)
{
    if (primes_sz < MAX_PRIMES)
        primes[primes_sz++] = p;
}

/* --------------------------------------------------------------------- */
/*  Generate the next qualifying prime and store it in the cache.      */
/* --------------------------------------------------------------------- */
static void generate_next(void)
{
    while (true)
    {
        if (is_prime(next_candidate))
        {
            add_prime(next_candidate);

            uint64_t p = next_candidate;
            unsigned pop   = __builtin_popcountll(p);
            unsigned bitlen = 64 - __builtin_clzll(p);   /* bitlen for p>0 */

            if ((pop << 1) <= bitlen)   /* 2*pop ≤ bitlen  */
            {
                if (cache_sz < MAX_CACHE)
                    cache[cache_sz++] = (seq_t)p;
                ++next_candidate;      /* move to the next integer for future calls */
                return;                /* finished generating one term */
            }
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
/*  The requested sequence value.  The function works for any order of
   requests (memoizes internally).  a(1) corresponds to the first term.  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for 0, return 0 as guard */

    while (cache_sz < n)
        generate_next();

    return cache[n - 1];
}
