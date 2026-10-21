/*
 * A094929  Let p = n-th prime; a(n) = smallest odd prime q such that p is not
 *          a square mod q.
 * https://oeis.org/A094929
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd prime q such that the n-th prime p is NOT a
 *            quadratic residue modulo q (Legendre symbol (p/q) = -1), n ≥ 3.
 *            The offset is 3 in the OEIS data
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   static parameters
------------------------------------------------------------------*/
#define PRIME_BOUND 2000000ULL   /* bound for the sieve (≈148,933 primes) */

static uint32_t *prime_list   = NULL;   /* primes up to PRIME_BOUND      */
static size_t    prime_count  = 0;       /* number of primes in prime_list*/
static seq_t  *a_cache        = NULL;   /* cache for a(n)                 */
static size_t  a_cache_cap    = 0;       /* allocated capacity of a_cache  */
static size_t  a_cache_len    = 0;       /* highest index computed + 1     */

/* ------------------------------------------------------------------
   power modulo using 128‑bit intermediate to avoid overflow
------------------------------------------------------------------*/
static inline uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e)
    {
        if (e & 1ULL)
            r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

/* ------------------------------------------------------------------
   sieve of Eratosthenes up to PRIME_BOUND
------------------------------------------------------------------*/
static void generate_primes(void)
{
    if (prime_list != NULL) return;      /* already generated */

    size_t N = PRIME_BOUND;
    uint8_t *sieve = (uint8_t *)malloc(N + 1);
    if (!sieve) return;                 /* out of memory – abort */

    for (size_t i = 0; i <= N; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= N; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= N; j += i)
                sieve[j] = 0;

    /* Count primes */
    for (size_t i = 2; i <= N; ++i)
        if (sieve[i]) ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= N; ++i)
        if (sieve[i]) prime_list[idx++] = (uint32_t)i;

    free(sieve);
}

/* ------------------------------------------------------------------
   ensure the cache has capacity at least ‘needed’
------------------------------------------------------------------*/
static void ensure_a_cache_size(size_t needed)
{
    if (a_cache_cap >= needed) return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* allocation failure – keep old cache */
    a_cache = tmp;
    for (size_t i = a_cache_cap; i < new_cap; ++i) a_cache[i] = (seq_t)0;
    a_cache_cap = new_cap;
}

/* ------------------------------------------------------------------
   main sequence function
------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;  /* behaviour below offset is arbitrary */

    /* generate primes only once */
    generate_primes();

    /* ensure enough space for requested index */
    ensure_a_cache_size((size_t)n + 1);

    /* compute values lazily up to n */
    for (size_t i = a_cache_len; i <= (size_t)n; ++i)
    {
        if (i < 3) { a_cache[i] = (seq_t)0; continue; }

        /* nth prime (1‑based indexing) */
        uint32_t p = prime_list[i - 1];   /* prime_list[0] = 2 */

        /* find smallest odd prime q with (p/q) = -1 */
        uint32_t q = 0;
        for (size_t j = 1; j < prime_count; ++j) /* start from 3 */
        {
            uint32_t qp = prime_list[j];
            uint64_t exp = ((uint64_t)qp - 1ULL) >> 1;   /* (q-1)/2 */
            uint64_t res = powmod64(p, exp, qp);
            if (res == qp - 1ULL) { q = qp; break; }
        }
        a_cache[i] = (seq_t)q;
    }

    a_cache_len = (size_t)n + 1;
    return a_cache[n];
}
