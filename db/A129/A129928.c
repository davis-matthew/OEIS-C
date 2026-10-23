/*
 * A129928  Primes equal to 3*Sp - 2, where Sp is a semiprime.
 * https://oeis.org/A129928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*Sp - 2, where Sp is the n‑th semiprime for which 3*Sp-2
 *            is prime
 * Range:     u8 n <= 9, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime sieve – to generate primes needed for the sequence          */
/* ------------------------------------------------------------------ */

#define PRIME_LIMIT 20000000UL           /* 20 million – enough for n≈10000 */

/* Global storage for the sieve */
static uint8_t *is_composite = NULL;     /* 1 = composite, 0 = prime candidate */
static uint64_t *primes      = NULL;     /* array of primes */
static size_t    primes_cnt  = 0;        /* number of primes found */
static bool      primes_init = false;   /* flag */

/* Build the sieve and fill the primes array */
static void build_prime_sieve(void)
{
    if (primes_init) return;

    is_composite = (uint8_t*)calloc(PRIME_LIMIT + 1, 1);
    if (!is_composite) exit(EXIT_FAILURE);

    /* Rough estimate of prime count : <limit / ln(limit) */
    size_t est = PRIME_LIMIT / 10 + 1;
    primes = (uint64_t*)malloc(est * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == est) {
                est <<= 1;          /* double buffer when full */
                uint64_t *tmp = (uint64_t*)realloc(primes, est * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }

    /* Shrink to actual size */
    uint64_t *tmp = (uint64_t*)realloc(primes, primes_cnt * sizeof(uint64_t));
    if (tmp) primes = tmp;

    primes_init = true;
}

/* ------------------------------------------------------------------ */
/*  Helper to test whether a number k is a semiprime (product of
    exactly two prime factors, counting multiplicity).                     */
/* ------------------------------------------------------------------ */
static bool is_semiprime(uint64_t k)
{
    if (k < 4) return false;   /* smallest semiprime is 4 = 2*2 */

    size_t cnt = 0;
    uint64_t tmp = k;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt > 2) return false;
        }
    }
    if (tmp > 1) ++cnt;

    return cnt == 2;
}

/* ------------------------------------------------------------------ */
/*  Cache for already computed terms                                 */
/* ------------------------------------------------------------------ */
static seq_t *term_cache = NULL;   /* holds a(1), a(2), … */
static size_t term_len   = 0;      /* number of terms stored */
static size_t term_cap   = 0;      /* allocated capacity */
static size_t prime_idx  = 0;      /* next prime to examine */

/* Ensure capacity for term_cache */
static void ensure_term_capacity(size_t need)
{
    if (term_cap >= need) return;

    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    term_cache = tmp;
    term_cap   = new_cap;
}

/* Generate terms until we have at least n of them */
static void generate_until(idx_t n)
{
    build_prime_sieve();
    ensure_term_capacity((size_t)n);

    while (term_len < n) {
        if (prime_idx >= primes_cnt) {
            /* In practice for n≈10000 this should never happen
               with PRIME_LIMIT = 20 million. */
            exit(EXIT_FAILURE);
        }

        uint64_t p = primes[prime_idx++];
        if ((p + 2) % 3 != 0) continue;             /* (p+2)/3 must be integer */

        uint64_t k = (p + 2) / 3;
        if (is_semiprime(k)) {
            term_cache[term_len++] = (seq_t)p;       /* store as seq_t */
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* guard for n<1 – undefined by offset */
    generate_until(n);
    return term_cache[n - 1];
}
