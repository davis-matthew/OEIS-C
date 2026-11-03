/*
 * A164042  Primes p such that 2*p^2+4*p+1 is also prime.
 * https://oeis.org/A164042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 835, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p such that 2*p^2 + 4*p + 1 is also prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static uint32_t *prime_nums = NULL;      /* primes up to PRIME_LIMIT           */
static size_t   prime_count       = 0;
static size_t   prime_idx          = 0;   /* next prime to test                */

static seq_t *seq_cache = NULL;          /* cached qualifying primes          */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

#define PRIME_LIMIT 20000000U          /* 20 million enough for n≈10000      */

/* ------------------------------------------------------------------ */
/* Generate all primes up to PRIME_LIMIT using the Eratosthenes sieve. */
static void init_primes(void)
{
    if (prime_nums != NULL)
        return;                          /* already initialized */

    /* sieve array of bits (one byte per entry is fine for this size) */
    uint8_t *sieve = (uint8_t *)malloc((size_t)PRIME_LIMIT + 1);
    if (sieve == NULL)
        return;                          /* allocation failed – unlikely */

    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    /* count primes */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            ++prime_count;

    prime_nums = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (prime_nums == NULL) {
        free(sieve);
        return;
    }

    size_t pos = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            prime_nums[pos++] = i;

    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Grow the sequence cache if needed. */
static void ensure_seq_capacity(void)
{
    if (seq_len < seq_cap)
        return;
    seq_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
}

/* ------------------------------------------------------------------ */
/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* caller guarantees n>=1 */

    init_primes();

    if (seq_cache == NULL) {
        seq_cache = mem_new(seq_t, 16);
        seq_cap   = 16;
        seq_len   = 0;
    }

    /* Build the sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        if (prime_idx >= prime_count) {
            /* Exhausted pre‑computed primes – this should not happen
               for the intended n range (≈10 000). */
            break;
        }

        seq_t p = (seq_t)prime_nums[prime_idx++];
        /* q = 2*p^2 + 4*p + 1 */
        seq_t q = p * p;
        q = q * 2 + p * 4 + 1;

        if (isPrimeGeneric(q)) {
            ensure_seq_capacity();
            seq_cache[seq_len++] = p;
        }
    }

    return seq_cache[(size_t)n - 1];
}
