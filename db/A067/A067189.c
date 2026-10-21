/*
 * A067189  Numbers that can be expressed as the sum of two primes in exactly
 *          three ways.
 * https://oeis.org/A067189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A067189: the even numbers expressible as the
   sum of two primes in exactly three distinct ways.  The function
   generates primes on demand and caches results up to the requested
   index.  It works for any seq_t and idx_t type supplied by the
   build. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- State ----------------------------------------------------------- */

static seq_t *cache      = NULL;      /* cached values of the sequence */
static size_t cache_len  = 0;        /* number of cached terms */
static size_t cache_cap  = 0;        /* cache capacity */

static bool      *is_prime   = NULL; /* primality sieve */
static size_t     prime_limit = 0;    /* maximum number sieved */
static uint32_t  *prime_list = NULL; /* list of primes up to prime_limit */
static size_t     prime_count = 0;   /* number of primes in prime_list */

/* next candidate even number */
static uint32_t next_even = 22;

/* --------------------------------------------------------------------- */
/* Cache helpers */

static void append_cache(seq_t val)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Sieve helpers */

static void ensure_sieve(size_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Allocate and build a new sieve up to 'limit'. */
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) abort();
    for (size_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (size_t j = p * p; j <= limit; j += p)
                sieve[j] = false;

    /* Count primes for allocation of prime_list. */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++count;

    uint32_t *plist = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!plist) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i])
            plist[idx++] = (uint32_t)i;

    /* Replace old structures. */
    free(is_prime);
    free(prime_list);

    is_prime   = sieve;
    prime_list = plist;
    prime_count = count;
    prime_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Goldbach representation counter (stop after more than 3). */

static uint8_t count_goldbach(uint32_t N)
{
    uint8_t cnt = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint32_t p = prime_list[i];
        if (p > N / 2)
            break;
        if (is_prime[N - p])
        {
            ++cnt;
            if (cnt > 3)
                return cnt;
        }
    }
    return cnt;
}

/* --------------------------------------------------------------------- */
/* Public sequence function */

seq_t a(idx_t n)
{
    /* Generate terms until we have 'n' cached values. */
    while (cache_len < (size_t)n)
    {
        /* Ensure the sieve covers the current candidate. */
        if ((size_t)next_even > prime_limit)
            ensure_sieve((size_t)next_even);

        if (count_goldbach((uint32_t)next_even) == 3)
            append_cache((seq_t)next_even);

        next_even += 2; /* only even candidates */
    }
    return cache[n - 1];
}
