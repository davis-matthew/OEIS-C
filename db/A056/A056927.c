/*
 * A056927  Difference between n^2 and largest prime less than n^2.
 * https://oeis.org/A056927
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - largest prime < n^2 (offset 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve of Eratosthenes up to the
   maximum n^2 that has been requested so far.  For each call we
   locate the first prime below n^2 by scanning downward, which
   typically requires only a few steps because primes are about
   log(n^2) apart.  The sieve stores only odd numbers in a bitset
   to keep the memory footprint below ~12.5 MB for n up to 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

/* Global sieve state */
static uint64_t sieve_limit = 0;       /* maximum number that has been sieved                     */
static uint8_t  *sieve_bits  = NULL;   /* bitset: bit set => composite number (odd only)         */

/* ---- Utility bitset access ---- */
static inline bool sieve_get(uint64_t n)
{
    if (n < 2) return false;               /* 0,1 are not prime, but not in the set                */
    if ((n & 1) == 0) return true;         /* even >2 are composite                                 */
    uint64_t idx = n >> 1;                 /* only odd numbers are stored (n = 2*idx+1)             */
    return (sieve_bits[idx >> 3] >> (idx & 7)) & 1U;
}

static inline void sieve_set(uint64_t n)
{
    if (n < 2) return;
    if ((n & 1) == 0) return;              /* even numbers are not stored                           */
    uint64_t idx = n >> 1;
    sieve_bits[idx >> 3] |= (1U << (idx & 7));
}

/* ---- Build OR rebuild the sieve up to 'limit' inclusive ---- */
static void build_sieve(uint64_t limit)
{
    /* free old memory if any once we have the new size */
    if (sieve_bits) free(sieve_bits);

    /* only odd numbers are stored, one bit per odd number */
    uint64_t odd_count = (limit >> 1) + 1;  /* include 1 as odd for index safety */
    size_t   byte_size = (odd_count >> 3) + 1;
    sieve_bits = (uint8_t *)calloc(byte_size, 1);
    if (!sieve_bits) exit(EXIT_FAILURE);   /* out of memory */

    /* Sieve of Eratosthenes over odd numbers only */
    uint64_t root = (uint64_t)(sqrt((double)limit) + 0.5);
    for (uint64_t i = 3; i <= root; i += 2)
    {
        if (!sieve_get(i))
        {
            uint64_t step = i << 1;               /* skip even multiples */
            for (uint64_t j = i * i; j <= limit; j += step)
                sieve_set(j);
        }
    }

    sieve_limit = limit;
}

/* ---- Ensure sieve covers all numbers up to 'limit' ---- */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;
    build_sieve(limit);
}

/* ---- Simple prime test using the sieve ---- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;        /* even >2 are not prime */
    return !sieve_get(n);
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    /* Offset is 2: values for n<2 are undefined, but we return 0 */
    if (n < 2) return (seq_t)0;

    /* Compute m = n^2 in seq_t and as idx_t for sieve use */
    seq_t m_seq = (seq_t)n * (seq_t)n;
    uint64_t m_val = (uint64_t)n * (uint64_t)n;   /* fits easily for common test ranges */

    /* Prepare sieve up to m-1 (largest candidate prime) */
    ensure_sieve(m_val - 1);

    /* Search downward for the first prime below m */
    uint64_t candidate = m_val - 1;
    while (candidate >= 2 && !is_prime_uint64(candidate))
        --candidate;

    /* Compute difference */
    return m_seq - (seq_t)candidate;
}
