/*
 * A124106  Octagonal numbers equal to S*(3S - 2) with 3S - 2 = k^2 and S
 *          semiprime.
 * https://oeis.org/A124106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A124106: Octagonal numbers equal to S*(3S-2) with 3S-2 = k²
   and S semiprime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- global data ----------------------------------------------------- */

#define PRIME_LIMIT 1000000   /* enough for k up to ~10⁵, covers n≈10⁴ */

static int *g_primes     = NULL;  /* prime list */
static size_t g_prime_cnt = 0;

static seq_t *g_cache    = NULL;  /* computed terms */
static size_t g_cache_used = 0;
static size_t g_cache_alloc = 0;

static idx_t g_next_k  = 1;       /* next k to test (k ≥ 1) */

/* ---- helper functions ---------------------------------------------- */

/* simple sieve of Eratosthenes; called once */
static void init_primes(void)
{
    if (g_primes != NULL) return;

    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (is_prime == NULL) abort();   /* out of memory */

    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    for (unsigned int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (unsigned int j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* count primes first */
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++g_prime_cnt;

    g_primes = (int*)malloc(g_prime_cnt * sizeof(int));
    if (g_primes == NULL) abort();

    size_t idx = 0;
    for (unsigned int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) g_primes[idx++] = (int)i;

    free(is_prime);
}

/* returns true iff n is the product of exactly two (not necessarily distinct)
   prime factors (i.e. semiprime).  n fits in uint64_t. */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;           /* 4 = 2*2 is the smallest semiprime */

    size_t count = 0;
    for (size_t i = 0; i < g_prime_cnt && (uint64_t)g_primes[i] * g_primes[i] <= n; ++i)
    {
        int p = g_primes[i];
        while (n % p == 0)
        {
            ++count;
            if (count > 2) return false;
            n /= p;
        }
    }
    if (n > 1) ++count;                 /* remaining prime factor > sqrt(original n) */
    return count == 2;
}

/* ensures g_cache contains at least *need* terms (0‑based index). */
static void ensure_cache(size_t need)
{
    if (g_cache_used >= need) return;

    init_primes();

    while (g_cache_used < need)
    {
        /* advance k to next integer such that 3S-2 = k² is square with
           k not divisible by 3 (otherwise 3S-2 would be divisible by 3) */
        do ++g_next_k; while (g_next_k % 3 == 0);

        uint64_t k2 = (uint64_t)g_next_k * (uint64_t)g_next_k;
        uint64_t S  = (k2 + 2) / 3;     /* integer because k² ≡ 1 (mod 3) */

        if (!is_semiprime(S)) continue; /* not a valid S */

        /* value of the octagonal number */
        seq_t term = (seq_t)S * (seq_t)k2;  /* use seq_t multiplication */

        /* grow cache if needed */
        if (g_cache_used + 1 > g_cache_alloc)
        {
            size_t new_alloc = g_cache_alloc ? g_cache_alloc << 1 : 16;
            seq_t *tmp = (seq_t*)realloc(g_cache, new_alloc * sizeof(seq_t));
            if (tmp == NULL) abort();
            g_cache = tmp;
            g_cache_alloc = new_alloc;
        }
        g_cache[g_cache_used++] = term;
    }
}

/* ---- public function ------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* out of range – not used by tests */
    ensure_cache((size_t)n);
    return g_cache[(size_t)n - 1];
}
