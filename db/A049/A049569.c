/*
 * A049569  Primes p such that x^37 = 2 has a solution mod p.
 * https://oeis.org/A049569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that there exists x with x^37 ≡ 2
 *            (mod p)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This holds for all primes p with 37 ∤ (p-1); if p ≡ 1 (mod 37) then it requires 2^((p-1)/37) ≡ 1 (mod p). */
/* Includes 2 as a trivial solution. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

static seq_t *prime_list   = NULL;   /* array of primes satisfying the condition */
static size_t prime_count   = 0;      /* number of stored primes */
static size_t prime_cap     = 0;      /* capacity of prime_list */
static bool initialized    = false;  /* initialized flag */

/* Precompute primes up to a fixed limit that yields enough terms for n≈10000 */
static void init_sequence(void)
{
    if (initialized)
        return;

    /* Sieve limit – large enough to collect >40000 such primes */
    const uint64_t sieve_limit = 500000;          /* 5×10^5 is easily handled */
    bool *is_comp = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    if (!is_comp)
        return;  /* allocation failed (unlikely in problem constraints) */

    for (uint64_t i = 2; i * i <= sieve_limit; ++i)
        if (!is_comp[i])
            for (uint64_t j = i * i; j <= sieve_limit; j += i)
                is_comp[j] = true;

    /* Iterate all primes up to sieve_limit */
    for (uint64_t p = 2; p <= sieve_limit; ++p)
    {
        if (is_comp[p])
            continue;                     /* not a prime */

        bool include = false;

        if (p == 2)                           /* 2 is trivial */
            include = true;
        else if ((p - 1) % 37 != 0)           /* 37 does not divide p-1 */
            include = true;
        else
        {
            uint64_t exp = (p - 1) / 37;
            seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
            if (r == (seq_t)1)
                include = true;
        }

        if (include)
            append_candidate(&prime_list, &prime_count, &prime_cap, (seq_t)p);
    }

    free(is_comp);
    initialized = true;
}

/* Public accessor: return the nth term (n >= 1).  Assumes n is within range. */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t safely – n fits into 64-bit */
    const size_t idx = (size_t)n;

    init_sequence();

    /* Ensure that we have at least idx terms;  with sieve_limit=500000 we should */
    if (idx == 0 || idx > prime_count)
        return (seq_t)0;  /* out of bounds – application may handle this */

    return prime_list[idx - 1];
}
