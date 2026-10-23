/*
 * A106349  Primes indexed by semiprimes.
 * https://oeis.org/A106349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(semiprime(n))
 * Range:     u8 n <= 18, u16 n <= 1763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime whose index equals the n‑th semiprime. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/*  Parameters – tune so that all required terms fit.                         */
/* ------------------------------------------------------------------------- */
#define SEMI_MAX_VAL 2000000          /* largest semiprime to pre‑compute   */
#define SEMI_MAX_COUNT 400000         /* upper bound on number of semiprimes */
#define PRIME_MAX_VAL 35000000        /* sieve limit for primes              */

/* ------------------------------------------------------------------------- */
/*  Static storage                                                      */
/* ------------------------------------------------------------------------- */
static uint32_t spf[SEMI_MAX_VAL + 1];          /* smallest prime factor      */
static uint32_t semiprimes[SEMI_MAX_COUNT];    /* nth semiprime              */
static size_t semiprime_count = 0;              /* how many semiprimes stored */

static uint8_t prime_flag[PRIME_MAX_VAL + 1];   /* sieve flag                 */
static uint32_t *primes = NULL;                /* array of primes            */
static size_t prime_count = 0;                  /* number of primes found     */

/* ------------------------------------------------------------------------- */
/*  Helper: small‑omega – total multiplicity of prime factors             */
/* ------------------------------------------------------------------------- */
static inline size_t omega_total(uint32_t n)
{
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    return cnt;
}

/* ------------------------------------------------------------------------- */
/*  One‑time sieve initialisation                                         */
/* ------------------------------------------------------------------------- */
static void init_sieve(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* 1. Compute spf for 2..SEMI_MAX_VAL */
    for (size_t i = 2; i <= SEMI_MAX_VAL; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            for (size_t j = i; j <= SEMI_MAX_VAL; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* 2. Generate all semiprimes (two prime factors, multiplicity =2) */
    semiprime_count = 0;
    for (uint32_t n = 4; n <= SEMI_MAX_VAL && semiprime_count < SEMI_MAX_COUNT; ++n) {
        if (omega_total(n) == 2) semiprimes[semiprime_count++] = n;
    }

    /* 3. Sieve primes up to PRIME_MAX_VAL */
    for (size_t i = 2; i <= PRIME_MAX_VAL; ++i) prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;

    for (size_t i = 2; i * i <= PRIME_MAX_VAL; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= PRIME_MAX_VAL; j += i) prime_flag[j] = 0;
        }
    }

    /* Count primes so we can allocate array */
    for (size_t i = 2; i <= PRIME_MAX_VAL; ++i)
        if (prime_flag[i]) ++prime_count;

    primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_MAX_VAL; ++i)
        if (prime_flag[i]) primes[idx++] = (uint32_t)i;
}

/* ------------------------------------------------------------------------- */
/*  Sequence function                                                      */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure data structures are ready */
    init_sieve();

    /* Guard against invalid indices (n <= 0) */
    if (n == 0) return (seq_t)0;

    /* If the requested index is larger than we pre‑computed, fail silently */
    if ((size_t)n > semiprime_count) return (seq_t)0;

    /* semiprime(n) gives the index that we need to look up in the prime list */
    uint32_t k = semiprimes[(size_t)n - 1];   /* 1‑based index of prime */

    /* primes[] is 0‑based (prime(1)==2 is primes[0]) */
    if (k == 0) return (seq_t)0;              /* safety check */
    return (seq_t)primes[(size_t)k - 1];
}
