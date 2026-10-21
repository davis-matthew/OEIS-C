/*
 * A077064  Squarefree numbers of form prime - 1.
 * https://oeis.org/A077064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077064: squarefree numbers of the form prime‑1.
   The implementation builds a sieve of primes up to a fixed limit
   and stores values p‑1 whenever p‑1 is squarefree.  Results are cached
   so repeated calls are O(1) after the cache is filled up to the
   requested index. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime sieve.  SIEVE_LIMIT is large enough for n ≈ 10 000. */
static const idx_t SIEVE_LIMIT = 2000000;          /* up to 2·10⁶ */
static uint8_t *sieve = NULL;                     /* 0 → prime, 1 → composite */
static uint32_t *prime_list = NULL;               /* list of primes */
static size_t prime_cnt = 0;                      /* number of primes found */

/* --------------------------------------------------------------------- */
/* Value cache for a(n). */
static seq_t *value_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cnt  = 0;
static size_t prime_ptr  = 0;        /* next prime to process during build */

/* --------------------------------------------------------------------- */
/* Initialize the sieve and prime list. */
static void init_sieve(void)
{
    if (sieve != NULL) return;   /* already done */

    sieve = (uint8_t *)calloc((size_t)SIEVE_LIMIT + 1, 1);
    if (!sieve) abort();

    /* Estimate number of primes: pi(n) < n / log n.  Allocate a bit more. */
    size_t est = SIEVE_LIMIT / 10;
    prime_list = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!prime_list) abort();

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve[i]) {
            /* store prime */
            if (prime_cnt == est) {
                est *= 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_list,
                                                    est * sizeof(uint32_t));
                if (!tmp) abort();
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = (uint32_t)i;

            if (i * i <= SIEVE_LIMIT) {
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve[j] = 1;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Test whether m is squarefree using the prime list (trial division). */
static bool is_squarefree(uint64_t m)
{
    /* Only need primes up to sqrt(m). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        uint64_t sq = p * p;
        if (sq > m) break;
        if (m % sq == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Ensure the cache has room for at least 'need' entries. */
static void ensure_cache_size(size_t need)
{
    if (cache_size >= need) return;
    size_t new_size = cache_size ? cache_size * 2 : 128;
    if (new_size < need) new_size = need;
    seq_t *tmp = (seq_t *)realloc(value_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    value_cache = tmp;
    cache_size = new_size;
}

/* --------------------------------------------------------------------- */
/* Build cache up to index n (1‑based). */
static void build_up_to(idx_t n)
{
    if (cache_cnt >= (size_t)n) return;   /* already have it */

    init_sieve();

    ensure_cache_size((size_t)n);

    while (cache_cnt < (size_t)n) {
        if (prime_ptr >= prime_cnt) {
            /* We expected SIEVE_LIMIT to be large enough.  If not, abort. */
            abort();
        }
        uint32_t p = prime_list[prime_ptr++];
        uint64_t m = (uint64_t)p - 1ULL;
        if (is_squarefree(m)) {
            value_cache[cache_cnt++] = (seq_t)m;   /* truncated if necessary */
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public function as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but harmless */
    build_up_to(n);
    return value_cache[n - 1];
}
