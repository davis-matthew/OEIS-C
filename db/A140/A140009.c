/*
 * A140009  Primes of the form 24x^2+24xy+61y^2.
 * https://oeis.org/A140009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140009: primes of the form 24x²+24xy+61y²
 * The function returns the n‑th prime that can be represented as
 * 24x²+24xy+61y².  The primes are exactly those congruent to
 * {61, 109, 349, 469, 541, 589, 901, 1069, 1141, 1261} (mod 1320).
 *
 * The implementation builds a sieve up to 15 000 000, stores all
 * primes, and extracts those in the allowed residue classes.  It
 * memoises the sequence so that calls in any order are efficient.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Allowed residue classes modulo 1320 */
static const unsigned int ALLOWED_RESIDUES[10] = {
    61, 109, 349, 469, 541, 589, 901, 1069, 1141, 1261
};
#define MODULO 1320
static bool allowed_mod[MODULO] = {0};

/* Maximum bound for the sieve (pre‑computed to contain >10 000 terms) */
#define MAX_PRECOMP_PRIME 15000000U

/* Sieve data */
static bool *sieve_is_composite = NULL;   /* 0 = prime, 1 = composite */
static size_t sieve_size = 0;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Sequence memoisation */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Pointers into the prime list, persistent across calls */
static size_t next_prime_idx = 0;

/* ------------------------------------------------------------ */
/* initialise the allowed_mod table                                 */
static void init_allowed_mod(void)
{
    for (size_t i = 0; i < MODULO; ++i)
        allowed_mod[i] = false;
    for (size_t i = 0; i < 10; ++i)
        allowed_mod[ALLOWED_RESIDUES[i]] = true;
}

/* ------------------------------------------------------------ */
/* build a sieve of Eratosthenes up to MAX_PRECOMP_PRIME              */
static void build_sieve(void)
{
    if (sieve_is_composite != NULL)
        return;                      /* already built */

    sieve_size = MAX_PRECOMP_PRIME + 1;
    sieve_is_composite = (bool *)malloc(sieve_size);
    if (!sieve_is_composite)
        return;                      /* allocation failed – unlikely */

    /* Mark all as prime initially */
    for (size_t i = 0; i < sieve_size; ++i)
        sieve_is_composite[i] = false;

    /* 0 and 1 are not prime */
    if (sieve_size > 0) sieve_is_composite[0] = true;
    if (sieve_size > 1) sieve_is_composite[1] = true;

    /* Eratosthenes sieve */
    for (size_t i = 2; i * i <= MAX_PRECOMP_PRIME; ++i)
    {
        if (!sieve_is_composite[i])
        {
            for (size_t j = i * i; j <= MAX_PRECOMP_PRIME; j += i)
                sieve_is_composite[j] = true;
        }
    }

    /* Count primes */
    for (size_t i = 2; i <= MAX_PRECOMP_PRIME; ++i)
        if (!sieve_is_composite[i])
            ++prime_count;

    /* Allocate and fill the prime list */
    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list)
        return;                      /* allocation failed – unlikely */

    size_t idx = 0;
    for (size_t i = 2; i <= MAX_PRECOMP_PRIME; ++i)
        if (!sieve_is_composite[i])
            prime_list[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------ */
/* ensure the sequence cache has space for at least `needed` terms */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap == 0 ? 128 : seq_cap << 1;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ */
/* main sequence routine                                           */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined – return 0 for safety */
        return (seq_t)0;

    /* one‑time global initialisation */
    static bool initialized = false;
    if (!initialized)
    {
        init_allowed_mod();
        build_sieve();
        seq_cache = NULL;
        seq_cap   = 0;
        seq_len   = 0;
        next_prime_idx = 0;
        initialized = true;
    }

    /* Generate terms until we reach the requested index */
    while (seq_len < (size_t)n)
    {
        /* Scan primes once, adding those in allowed residue classes */
        while (next_prime_idx < prime_count && seq_len < (size_t)n)
        {
            uint32_t p = prime_list[next_prime_idx++];
            if (allowed_mod[p % MODULO])
            {
                ensure_cache_capacity(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)p;
            }
        }

        /* If we run out of pre‑computed primes, the limit is too small
           – this should not happen for n ≤ 10 000 with the chosen bound. */
        if (seq_len < (size_t)n)
            break;
    }

    return seq_cache[(size_t)n - 1];
}
