/*
 * A048946  Numbers k such that the product of divisors of k is a fifth power.
 * https://oeis.org/A048946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2757, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A048946
   Numbers k such that the product of divisors of k is a fifth power.
   For each prime power p^a || k we check that a * τ(k) ≡ 0 (mod 10),
   where τ(k) is the divisor count of k.  The function searches
   integers in increasing order, caches all found terms, and returns
   the n‑th term (a(1) = 1).                                  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Prime sieve – generated once, up to a fixed limit.          */
/* ------------------------------------------------------------ */
#define LIMIT 4000000                               /* > sqrt of tested range */
#define PRIMES_BUF_SIZE 300000                      /* ≈ primes ≤ LIMIT */

static uint8_t  sieve[LIMIT + 1];
static uint32_t prime_buf[PRIMES_BUF_SIZE];
static size_t   primes_cnt   = 0;
static bool     primes_init = false;

static void build_primes(void)
{
    size_t pc = 0;
    for (size_t i = 2; i * i <= (size_t)LIMIT; ++i)
        if (!sieve[i])
            for (size_t j = i * i; j <= (size_t)LIMIT; j += i)
                sieve[j] = 1;
    for (size_t i = 2; i <= (size_t)LIMIT; ++i)
        if (!sieve[i])
            prime_buf[pc++] = (uint32_t)i;
    primes_cnt   = pc;
    primes_init  = true;
}

/* ------------------------------------------------------------ */
/* Test whether a given integer n satisfies the criterion.     */
/* ------------------------------------------------------------ */
static bool good_divisor_prod(idx_t n)
{
    if (n == 1) return true;   /* 1 is admissible */

    uint64_t x = (uint64_t)n;
    int exps[64];
    int ecnt = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = prime_buf[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > x) break;
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            exps[ecnt++] = e;
        }
    }
    if (x > 1) exps[ecnt++] = 1;      /* remaining prime factor */

    /* τ(n) modulo 10 */
    int dmod = 1;
    for (int i = 0; i < ecnt; ++i)
        dmod = (dmod * (exps[i] + 1)) % 10;

    for (int i = 0; i < ecnt; ++i)
        if ( (exps[i] * dmod) % 10 != 0 )
            return false;
    return true;
}

/* ------------------------------------------------------------ */
/* Cached generation of the sequence.                          */
/* ------------------------------------------------------------ */
static seq_t  *cache     = NULL;
static idx_t   cached    = 0;      /* number of terms stored */
static size_t  cache_cap = 0;      /* current capacity of cache */
static uint64_t next_val   = 1;    /* next integer to check */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for 0, return 0 */

    if (!primes_init) build_primes();

    while (cached < n) {
        /* Grow cache if needed */
        if ((size_t)cached >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory */
            cache     = tmp;
            cache_cap = new_cap;
        }

        if (next_val == 1) {
            cache[(size_t)cached++] = (seq_t)1;
            ++next_val;
            continue;
        }

        if (good_divisor_prod((idx_t)next_val))
            cache[(size_t)cached++] = (seq_t)next_val;
        ++next_val;
    }

    return cache[(size_t)(n - 1)];
}
