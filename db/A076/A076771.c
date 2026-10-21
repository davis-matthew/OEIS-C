/*
 * A076771  Even numbers n representable as the sum of two non-coprime odd
 *          composites.
 * https://oeis.org/A076771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th even number n ≥ 18 such that φ(n) < (n‑4)/2.
 *            That condition is equivalent to n being representable as the sum
 *            of two odd composites with a common divisor. The function builds
 *            the sequence lazily and stores the terms in a static cache.
 *            Primes up to 200 000 are generated once and us...
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Prime generation (up to 200 000) – executed once on first call    */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list   = NULL;
static size_t     prime_cnt   = 0;
static size_t     prime_cap   = 0;
static int        primes_built = 0;

static void build_primes(void)
{
    const uint32_t LIMIT = 200000U;
    uint8_t *sieve = (uint8_t*)calloc(LIMIT + 1U, sizeof(uint8_t));
    if (!sieve) return;          /* out of memory – sequence will fail */
    sieve[0] = sieve[1] = 1;      /* mark 0,1 as composite */

    /* Simple sieve of Eratosthenes */
    for (uint32_t i = 2U; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;

    prime_cap = 10000U;           /* initial capacity for prime list */
    prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }

    for (uint32_t i = 2U; i <= LIMIT; ++i)
        if (!sieve[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1; /* double capacity */
                uint32_t *tmp = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
                if (!tmp) break;
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = i;
        }

    free(sieve);
    primes_built = 1;
}

/* ------------------------------------------------------------------ */
/*  Euler's φ function – computed on the fly using the prime list      */
/* ------------------------------------------------------------------ */
static inline uint64_t phi_u64(uint64_t n)
{
    uint64_t result = n;
    uint64_t tmp     = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            result -= result / p;
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache and generator                                         */
/* ------------------------------------------------------------------ */
static seq_t *term_cache   = NULL;
static size_t term_cap     = 0;
static size_t term_size    = 0;          /* number of valid terms */
static uint64_t last_even = 16U;         /* even number already tested */

/* Ensure that at least n terms are available in term_cache */
static void ensure_terms(idx_t n)
{
    if (term_size >= (size_t)n) return;

    /* Build primes if not already done */
    if (!primes_built) build_primes();

    if (!term_cache) {
        term_cap = 1000U;
        term_cache = (seq_t*)malloc(term_cap * sizeof(seq_t));
        if (!term_cache) return; /* memory error */
    }

    /* Produce terms until we have enough */
    while (term_size < (size_t)n) {
        last_even += 2U;          /* next even candidate */
        uint64_t ph = phi_u64(last_even);
        uint64_t rhs = (last_even - 4U) / 2U;   /* integer division */
        if (ph < rhs) {
            /* add to cache */
            if (term_size == term_cap) {
                term_cap <<= 1;   /* double capacity */
                seq_t *tmp = (seq_t*)realloc(term_cache, term_cap * sizeof(seq_t));
                if (!tmp) return;
                term_cache = tmp;
            }
            term_cache[term_size++] = (seq_t)last_even;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public API function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */
    ensure_terms(n);
    if (term_size < (size_t)n) return (seq_t)0; /* out of range */
    return term_cache[n - 1U];
}
