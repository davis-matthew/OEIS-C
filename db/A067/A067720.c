/*
 * A067720  Numbers k such that phi(k^2+1) = k*phi(k+1).
 * https://oeis.org/A067720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 752, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A067720
 *  Numbers k such that φ(k^2+1) = k*φ(k+1)
 *
 *  The condition is equivalent to k+1 being prime and (k^2+1) being prime,
 *  with the single exception k = 8.  The function generates the sequence
 *  by scanning k in increasing order, testing the two primality
 *  conditions, caching the results, and returning the n‑th term.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  64‑bit deterministic Miller–Rabin primality test (deterministic for
 *  all n < 2^64).  Uses the 12‑base set: 2,3,5,7,11,13,17,19,23,29,31,37.
 *  --------------------------------------------------------------------- */
static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1ULL;
    while (e)
    {
        if (e & 1ULL)
            r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t a = small_primes[i];
        if (a >= n)
            break;
        uint64_t x = powmod64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Eratosthenes sieve up to SIEVE_LIMIT.  Used for fast primality
 *  testing of k+1.  --------------------------------------------------------------------- */
#define SIEVE_LIMIT 20000000ULL          /* large enough for ~10 k terms */
static unsigned char *sieve_small = NULL;   /* 0 = composite, 1 = prime   */
static bool sieve_done = false;

static void init_sieve(void)
{
    sieve_small = (unsigned char *)malloc(SIEVE_LIMIT + 1);
    if (!sieve_small)
        abort();
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        sieve_small[i] = 1;
    sieve_small[0] = 0;
    sieve_small[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= SIEVE_LIMIT; ++i)
        if (sieve_small[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_small[j] = 0;
    sieve_done = true;
}

/* --------------------------------------------------------------------- */
/*  Dynamic cache of computed terms.  The cache grows geometrically.
 *  --------------------------------------------------------------------- */
static seq_t *term_cache = NULL;
static idx_t cache_len = 0;   /* number of terms stored */
static idx_t cache_cap = 0;   /* allocated capacity */

static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();             /* out of memory */
    term_cache = new_ptr;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  The main sequence function.  ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; not expected to be called */
        return 0;

    /* lazily initialize sieve */
    if (!sieve_done)
        init_sieve();

    /* generate terms until we have at least n of them */
    static idx_t last_k_checked = 0;      /* last candidate k checked */
    while (cache_len < n)
    {
        ++last_k_checked;                   /* next candidate k */

        /* special exception */
        if (last_k_checked == 8)
        {
            ensure_cache_cap(cache_len + 1);
            term_cache[cache_len++] = (seq_t)last_k_checked;
            continue;
        }

        /* test primality of k+1 */
        bool kp1_prime;
        if (last_k_checked + 1 <= SIEVE_LIMIT)
            kp1_prime = sieve_small[last_k_checked + 1];
        else
            kp1_prime = is_prime_64((uint64_t)(last_k_checked + 1));

        if (!kp1_prime)
            continue;           /* fails first test */

        /* test primality of k^2+1 (fits in 64 bits for our needed range) */
        unsigned __int128 sq = (unsigned __int128)last_k_checked;
        sq = sq * sq + 1;
        uint64_t sq64 = (uint64_t)sq;          /* safe if sq < 2^64 */
        if (!is_prime_64(sq64))
            continue;           /* fails second test */

        /* k satisfies the condition */
        ensure_cache_cap(cache_len + 1);
        term_cache[cache_len++] = (seq_t)last_k_checked;
    }

    return term_cache[n - 1];
}
