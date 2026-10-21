/*
 * A098835  Smith abundant numbers.
 * https://oeis.org/A098835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth Smith abundant number (OEIS A098835).
   The function scans integers starting from 2, testing the
   Smith and abundant conditions.  Results are cached so
   repeated calls are efficient.  The algorithm is O(c) where c
   is the size of the largest number examined (≈ 1 million for
   n up to 10 000).  */
/* --------------------------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Prime sieve (up to 1 000 000)                                         */
/* --------------------------------------------------------------------- */

static const uint32_t PRIME_SIEVE_LIMIT = 1000000U;
static uint32_t *prime_list      = NULL;   /* array of primes          */
static size_t    prime_count     = 0;       /* number of primes stored  */

static void init_prime_sieve(void)
{
    if (prime_list != NULL) return;        /* already initialised    */

    /* Allocate sieve array */
    char *sieve = (char*)malloc(PRIME_SIEVE_LIMIT + 1U);
    if (!sieve) return;                   /* allocation failed */
    for (uint32_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = 0;

    /* Sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[(size_t)j] = 1;
        }
    }
    /* Count primes and store them */
    prime_count = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!sieve[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = i;
    free(sieve);
}

/* --------------------------------------------------------------------- */
/* Utility: sum of decimal digits                                       */
/* --------------------------------------------------------------------- */

static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x) { sum += x % 10; x /= 10; }
    return sum;
}

/* --------------------------------------------------------------------- */
/* Test whether n is a Smith number and an abundant number.             */
/* --------------------------------------------------------------------- */

static bool is_smith_and_abundant(uint64_t n)
{
    if (n < 4)                /* smallest composite */
        return false;

    uint64_t sum_digits_n = digit_sum_uint64(n);

    /* Gamma: sum of digits of the prime factorisation (with multiplicity) */
    uint64_t sum_factor_digits = 0;
    /* Sigma: sum of all divisors, used to test abundance */
    unsigned __int128 sigma = 1;     /* product over prime powers */

    uint64_t rem = n;
    size_t pf_tot = 0;               /* total number of prime factors (with multiplicity) */

    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint32_t p = prime_list[i];
        if (rem % p == 0U) {
            uint32_t e = 0;
            while (rem % p == 0U) { rem /= p; ++e; }

            pf_tot += e;                 /* e occurrences of p */

            sum_factor_digits += digit_sum_uint64((uint64_t)p) * e;

            /* Compute (p^{e+1}-1)/(p-1) as a product of (1 + p + p^2 + ... + p^e) */
            unsigned __int128 p_pow = 1;
            unsigned __int128 term_sum = 1;      /* starts with 1 */
            for (uint32_t j = 0; j < e; ++j) {
                p_pow *= p;
                term_sum += p_pow;
            }
            sigma *= term_sum;
        }
    }

    /* Remaining prime factor > sqrt(n) */
    if (rem > 1U) {
        ++pf_tot;
        sum_factor_digits += digit_sum_uint64(rem);
        sigma *= (unsigned __int128)(1 + rem);
    }

    /* Composite iff total multiplicity >= 2 */
    if (pf_tot < 2U)
        return false;

    /* Abundant iff sum of proper divisors > n  ⇒  sigma > 2*n */
    if (sigma <= (unsigned __int128)(2ULL * n))
        return false;

    /* Smith iff digit sums match */
    if (sum_digits_n != sum_factor_digits)
        return false;

    return true;
}

/* --------------------------------------------------------------------- */
/* Cache for found Smith abundant numbers (stored as uint64_t).          */
/* --------------------------------------------------------------------- */

static uint64_t *cache            = NULL;   /* array of found numbers */
static size_t    cache_len        = 0;      /* number of cached values  */
static size_t    cache_cap        = 0;      /* allocated capacity       */
static uint64_t  next_candidate   = 2;      /* next number to test      */

static void grow_cache_if_needed(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2ULL : 16ULL;
    while (new_cap < needed) new_cap *= 2ULL;
#if defined(__cplusplus)
    uint64_t *tmp = new uint64_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    delete[] cache;
    cache = tmp;
#else
    uint64_t *tmp = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
    cache = tmp;
#endif
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset handling – not used for OEIS */
        return (seq_t)0;

    init_prime_sieve();

    /* Compute terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        while (!is_smith_and_abundant(next_candidate))
            ++next_candidate;

        grow_cache_if_needed(cache_len + 1u);
        cache[cache_len++] = next_candidate;
        ++next_candidate;
    }
    return (seq_t)cache[n - 1];
}
