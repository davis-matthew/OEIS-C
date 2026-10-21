/*
 * A085777  Semiprimes such that when they are concatenated with their
 *          reversal are semiprime.
 * https://oeis.org/A085777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085777: a(n) = the nth semiprime s such that
 * concat(s, reverse(s)) is also semiprime.  Computed by brute–force
 * search with memoisation. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- Prime sieve up to a fixed limit --------------------------------- */
#define PRIME_LIMIT 2000000
static uint32_t primes[150000];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    uint8_t *is_prime = (uint8_t *)malloc(PRIME_LIMIT + 1);
    memset(is_prime, 1, PRIME_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint32_t)i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = 0;
    primes_cnt = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;
    free(is_prime);
    primes_inited = true;
}

/* --- Utility functions ------------------------------------------------- */
static uint64_t reverse_uint64(uint64_t n)
{
    uint64_t r = 0;
    while (n > 0)
    {
        r = r * 10 + n % 10;
        n /= 10;
    }
    return r;
}

/* Count real prime factors (with multiplicity).  Returns true iff x is
 * a product of exactly two primes. */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 2*2=4 */
    int cnt = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i)
    {
        while (x % primes[i] == 0)
        {
            x /= primes[i];
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (x > 1)                       /* remaining prime factor */
    {
        ++cnt;
        if (cnt > 2) return false;
    }
    return cnt == 2;
}

/* --- Sequence state --------------------------------------------------- */
static seq_t *cached = NULL;   /* array of computed terms */
static size_t cached_len = 0;
static size_t cached_cap = 0;
static uint64_t next_candidate = 2;   /* next number to test */
static bool state_inited = false;

/* Grow cached array, if necessary. */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cached_cap) return;
    size_t new_size = new_cap > 0 ? new_cap : 16;
    seq_t *tmp = (seq_t *)realloc(cached, new_size * sizeof(seq_t));
    if (!tmp) exit(1);
    cached = tmp;
    cached_cap = new_size;
}

/* Find the next qualifying semiprime and append it to the cache. */
static void generate_next_term(void)
{
    while (true)
    {
        uint64_t s = next_candidate++;
        if (!is_semiprime(s)) continue;

        uint64_t rev = reverse_uint64(s);

        /* compute 10^digits(rev) */
        uint64_t pow10 = 1;
        uint64_t tmp = rev;
        if (tmp == 0) pow10 = 10; /* shouldn't happen for s>0 */
        else
            while (tmp > 0)
            {
                pow10 *= 10;
                tmp /= 10;
            }

        /* concatenated number s||rev */
        unsigned __int128 cat128 = (unsigned __int128)s * pow10 + rev;
        if (cat128 > UINT64_MAX) continue; /* too large to test */
        uint64_t cat = (uint64_t)cat128;
        if (is_semiprime(cat))
        {
            /* store the term */
            ensure_cache_cap(cached_len + 1);
            cached[cached_len++] = (seq_t)s;
            break;
        }
    }
}

/* --- Public API -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but handle gracefully */
    size_t idx = (size_t)(n - 1); /* zero‑based index */

    if (!state_inited)
    {
        init_primes();
        cached = NULL;
        cached_len = 0;
        cached_cap = 0;
        next_candidate = 2;
        state_inited = true;
    }

    while (cached_len <= idx)
        generate_next_term();

    return cached[idx];
}
