/*
 * A176256  Numbers of the form 4k+1 with least prime divisor of the form
 *          4m-1.
 * https://oeis.org/A176256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A176256.
   It searches for integers of the form 4k+1 whose smallest prime
   divisor is congruent to 3 (mod 4).  The result is memoised so that
   the function is efficient for any calling order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Helper: integer square root (floor) for uint64_t values.     */
/* ------------------------------------------------------------ */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* ------------------------------------------------------------ */
/* Prime generation up to a fixed large limit (200000).        */
/* ------------------------------------------------------------ */
static uint32_t *prime_list = NULL;   /* array of primes (uint32_t)   */
static size_t    prime_cnt  = 0;      /* number of primes in the list*/
static bool      primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    const uint64_t LIMIT = 200000;  /* sufficient for 10 000 terms          */
    char *sieve = (char *)malloc(LIMIT + 1);
    if (!sieve) abort();
    for (uint64_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* Estimate number of primes (~LIMIT / log(LIMIT)) and allocate. */
    size_t est = LIMIT / 10 + 1;
    uint32_t *pr = (uint32_t *)calloc(est, sizeof(uint32_t));
    if (!pr) abort();
    size_t cnt = 0;

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) {
            if (cnt == est) {
                est *= 2;
                pr = (uint32_t *)realloc(pr, est * sizeof(uint32_t));
                if (!pr) abort();
            }
            pr[cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    sieve[j] = 0;
            }
        }
    }
    free(sieve);
    prime_list = pr;
    prime_cnt  = cnt;
    primes_inited = true;
}

/* ------------------------------------------------------------ */
/* Check if a 4k+1 number has a smallest prime factor ≡3 (mod4).*/
/* ------------------------------------------------------------ */
static bool is_good(seq_t x)
{
    uint64_t u = (uint64_t)x;
    if (u <= 3) return false;                 /* 5, 9, ...; 5 is prime   */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > u) break;
        if (u % pp == 0) {
            return (p & 3U) == 3U;             /* 3 mod 4?                 */
        }
        if (u % p == 0) {
            return (p & 3U) == 3U;
        }
    }
    /* No divisor found – u is prime. */
    return false;
}

/* ------------------------------------------------------------ */
/* Memoised search: stores computed terms in a static cache.   */
/* ------------------------------------------------------------ */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;
static size_t cache_cap  = 0;
static idx_t  next_k     = 1;   /* k used to generate 4k+1 candidates */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* undefined for n=0          */

    init_primes();                           /* ensure prime list exists     */

    while (cache_size < n) {
        /* Generate next candidate 4k+1. */
        uint64_t cand64 = 4ULL * (uint64_t)next_k + 1ULL;
        seq_t cand = (seq_t)cand64;

        if (is_good(cand)) {
            /* Grow cache if necessary. */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_size++] = cand;
        }
        ++next_k;
    }
    return cache[n - 1];
}
