/*
 * A038910  Primes p such that 34 is not a square mod p.
 * https://oeis.org/A038910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd prime p such that 34 is not a quadratic
 *            residue mod p, i.e. Legendre(34, p) = -1
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* modular exponentiation for 64‑bit values */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e)
    {
        if (e & 1)
            r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

/* static state for memoization */
static seq_t *terms = NULL;      /* array of terms */
static size_t termslen = 0;      /* number of terms stored */
static size_t termscap = 0;      /* capacity of the array */
static uint64_t last_checked = 3;/* candidate to start searching from */

/* a(n) with external linkage */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* grow terms array as needed */
    while (termslen < (size_t)n)
    {
        /* find next odd prime > last_checked */
        uint64_t p = last_checked;
        if (p <= 2) p = 3;
        else if ((p & 1ULL) == 0) ++p;

        while (!is_prime_small((seq_t)p))
            p += 2;  /* only odd candidates */

        last_checked = p + 1;

        /* Legendre symbol (34/p) via Euler's criterion */
        uint64_t leg = modpow64(34ULL % p, (p - 1) >> 1, p);

        if (leg == p - 1)   /* 34 is a non‑residue mod p */
        {
            /* ensure capacity */
            if (termslen == termscap)
            {
                termscap = termscap ? termscap * 2 : 8;
                terms = (seq_t *)realloc(terms, termscap * sizeof(seq_t));
            }
            terms[termslen++] = (seq_t)p;
        }
    }

    return terms[n - 1];
}
