/*
 * A046349  Composite numbers with only palindromic prime factors.
 * https://oeis.org/A046349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Composite numbers with only palindromic prime factors
 * Range:     u8 n <= 90, u16 n <= 2395, u32 n <= 3020, u64 n <= 3020,
 *            u128 n <= 3020, big any n
 * Helpers:   _is_pal_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: well‑known helper functions are assumed to be available. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_is_pal_digit.h"

/* Precomputed limits */
#define PRIME_LIMIT 5000000ULL   /* 5 000 000 is sufficient for the first 10 000 terms */

/* Internal state */
static bool _init_done = false;
static size_t  _prime_count = 0;
static uint32_t *_primes = NULL;           /* List of primes up to PRIME_LIMIT */
static uint8_t  *_is_prime = NULL;         /* Boolean sieve for primes */
static uint8_t  *_is_palprime = NULL;      /* Boolean table: is prime and palindrome? */

/* Trial factorisation using the pre‑computed prime list.
   Returns true if n is a composite number whose prime factors are all palindromic. */
static bool _is_valid_candidate(uint64_t n)
{
    if (n < 4) return false;                /* composites start at 4 */
    if (n <= PRIME_LIMIT && _is_prime[(size_t)n]) return false; /* skip primes */

    uint64_t tmp = n;
    for (size_t i = 0; i < _prime_count && (uint64_t)_primes[i] * (uint64_t)_primes[i] <= tmp; ++i)
    {
        uint32_t p = _primes[i];
        if (tmp % p == 0)
        {
            if (!_is_palprime[p]) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)                            /* remaining prime factor > sqrt(n) */
    {
        /* tmp will not exceed PRIME_LIMIT in the expected range of the calls. */
        if (tmp <= PRIME_LIMIT)
        {
            if (!_is_palprime[(size_t)tmp]) return false;
        }
        else
        {
            /* We are outside the pre‑computed table: conservatively reject. */
            return false;
        }
    }
    return true;
}

/* Lazy initialisation of the static tables. */
static void _init(void)
{
    if (_init_done) return;
    _init_done = true;

    /* Allocate and fill sieve */
    _is_prime   = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    _is_palprime = (uint8_t *)calloc(PRIME_LIMIT + 1, 1); /* same size as is_prime */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) _is_prime[i] = 1;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
    {
        if (_is_prime[i])
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                _is_prime[j] = 0;
    }

    /* Build prime list and palprime table */
    _prime_count = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) if (_is_prime[i]) _prime_count++;
    _primes = (uint32_t *)malloc(_prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
    {
        if (_is_prime[i]) {
            _primes[idx++] = (uint32_t)i;
            if (_is_pal_digit(i)) _is_palprime[i] = 1;
        }
    }
}

/* Cache for already generated sequence elements */
typedef struct {
    size_t  size;   /* number of elements stored */
    size_t  cap;    /* capacity */
    seq_t *data;    /* array of values */
} _cache_t;

static _cache_t _cache = {0, 0, NULL};

static void _cache_reserve(size_t needed)
{
    if (needed <= _cache.cap) return;
    size_t newcap = _cache.cap ? _cache.cap << 1 : 8;
    while (newcap < needed) newcap <<= 1;
    seq_t *newdata = (seq_t *)realloc(_cache.data, newcap * sizeof(seq_t));
    _cache.data = newdata;
    _cache.cap  = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _init();      /* ensure tables are ready */

    if (n == 0) return 0;  /* undefined for n==0, but guard against underflow */

    _cache_reserve((size_t)n);

    /* Generate sequence values until we have at least n of them */
    static uint64_t _last_checked = 3;       /* last number tested */
    while (_cache.size < (size_t)n)
    {
        uint64_t cand = _last_checked + 1;
        while (!_is_valid_candidate(cand)) cand++;
        _last_checked = cand;
        _cache.data[_cache.size++] = (seq_t)cand;   /* cast to seq_t (truncation if needed) */
    }

    return _cache.data[n - 1];   /* zero‑based index */
}
