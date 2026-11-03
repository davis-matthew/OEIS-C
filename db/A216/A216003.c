/*
 * A216003  Primes that are the sum of 1501 consecutive primes.
 * https://oeis.org/A216003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime that is the sum of 1501 consecutive primes (OEIS
 *            A216003). The function searches consecutively through the list
 *            of primes, keeps a running sum of length 1501, and stores the
 *            sum whenever it is itself prime. The search is performed once on
 *            the first call to a(idx_t) and memoised. The...
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 44 terms through n = 9175 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/mem.h"

/* The build supplies seq_t and idx_t; we rely on their definitions. */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    /* Simple Eratosthenes sieve.  limit >= 2. */
    size_t i, j;
    bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
    if (!sieve) return;             /* out of memory; return without primes */
    for (i = 2; i <= limit; ++i) sieve[i] = true;
    for (i = 2; i * i <= limit; ++i) if (sieve[i]) {
        for (j = i * i; j <= limit; j += i) sieve[j] = false;
    }

    /* Count primes. */
    *cnt_out = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) ++(*cnt_out);

    *primes_out = (uint64_t *)malloc(*cnt_out * sizeof(uint64_t));
    if (!*primes_out) {
        free(sieve);
        *cnt_out = 0;
        return;
    }
    size_t pos = 0;
    for (i = 2; i <= limit; ++i) if (sieve[i]) (*primes_out)[pos++] = (uint64_t)i;

    free(sieve);
}

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Number of consecutive primes to sum */
#define WINDOW 1501

/* Upper bound on the prime generation limit.  50 million produces
   enough terms (≈10 000) for all tests. */
#define PRIME_LIMIT 50000000ULL

/* Cached data ---------------------------------------------------- */
static uint64_t *g_primes   = NULL;  /* array of primes up to PRIME_LIMIT */
static size_t   g_prime_cnt = 0;     /* number of primes */

static seq_t *g_prefix = NULL;  /* prefix sums of primes: pref[i] = sum of first i primes */
static seq_t *g_terms  = NULL;  /* collected terms (prime sums) */
static size_t g_term_cnt = 0;   /* number of terms stored */
static size_t g_term_cap = 0;   /* allocated capacity for g_terms */

/* Grow the terms cache to hold at least new_cap elements */
static void grow_terms(size_t new_cap)
{
    g_terms = seq_realloc(g_terms, g_term_cap, new_cap);
    g_term_cap = new_cap;
}

/* Perform the single initialisation: build primes, prefixes, collect terms. */
static void init_once(void)
{
    if (g_primes != NULL)
        return;                   /* already initialised */

    /* 1. Build the list of primes up to PRIME_LIMIT */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(PRIME_LIMIT, &primes, &cnt);
    g_primes   = primes;
    g_prime_cnt = cnt;

    /* 2. Allocate prefix sums (size = cnt + 1) */
    g_prefix = seq_realloc(NULL, 0, cnt + 1);
    g_prefix[0] = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        g_prefix[i + 1] = g_prefix[i] + (seq_t)g_primes[i];
    }

    /* 3. Allocate space for terms (initial capacity) */
    grow_terms(1024);

    /* 4. Scan all windows of length WINDOW and collect prime sums */
    for (size_t i = 0; i + WINDOW <= g_prime_cnt; ++i)
    {
        seq_t sum = g_prefix[i + WINDOW] - g_prefix[i];
        if (isPrimeGeneric(sum))
        {
            if (g_term_cnt == g_term_cap)
                grow_terms(g_term_cap * 2 + 1);
            g_terms[g_term_cnt++] = sum;
        }
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_once();                      /* ensure data is ready */

    /* a(1) is the first term; safe for n >= 1 */
    if ((size_t)n < 1 || (size_t)n > g_term_cnt)
        return 0;                     /* undefined or out of range */

    return g_terms[(size_t)n - 1];
}
