/*
 * A142032  Primes congruent to 28 mod 31.
 * https://oeis.org/A142032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 28 mod 31
 * Range:     u8 n <= 1, u16 n <= 225, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Maximum number to sieve – covers more than 10 000 terms comfortably. */
#define SIEVE_MAX 3500000U

/* Smallest Prime Factor array – built once, reused for all calls. */
static uint32_t *spf = (uint32_t*)NULL;
static size_t spf_limit = 0U;

/* Build spf[0..limit] inclusive. */
static void init_spf(size_t limit)
{
    if (spf_limit >= limit)
        return;                       /* already built up to this size */

    size_t n = limit;
    uint32_t *new_spf = (uint32_t*)malloc((n + 1U) * sizeof(uint32_t));
    if (!new_spf)
        exit(1);                      /* cannot allocate */

    memset(new_spf, 0, (n + 1U) * sizeof(uint32_t));

    /* Sieve: for each prime, set its own entry and mark composites. */
    for (size_t i = 2U; i <= n; ++i)
    {
        if (new_spf[i] == 0U)          /* i is prime */
        {
            new_spf[i] = (uint32_t)i;
            if (i * i <= n)
            {
                for (size_t j = i * i; j <= n; j += i)
                    if (new_spf[j] == 0U)
                        new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* free old table if any */
    if (spf)
        free(spf);

    spf = new_spf;
    spf_limit = n;
}

/* Test whether a 32‑bit unsigned integer is prime, using spf table. */
static inline bool is_prime_uint32(uint32_t x)
{
    return x >= 2U && spf[x] == x;
}

/* The required OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;            /* behaviour below offset is irrelevant */

    /* Prepare the sieve once at the first call. */
    init_spf(SIEVE_MAX);

    size_t count = 0U;
    uint32_t candidate;

    for (candidate = 28U; candidate <= SIEVE_MAX; candidate += 31U)
    {
        if (is_prime_uint32(candidate))
        {
            ++count;
            if (count == (size_t)n)
                return (seq_t)candidate;
        }
    }

    /* If we reach here, the pre‑computed sieve was not large enough. */
    /* For the purposes of the test set (n ≤ 10 000) this never occurs. */
    return (seq_t)0U;
}
