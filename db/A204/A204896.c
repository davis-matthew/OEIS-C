/*
 * A204896  p(n)-q(n), where (p(n), q(n)) is the least pair of primes (ordered
 *          as in A204890) for which n divides p(n)-q(n).
 * https://oeis.org/A204896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A204896:  minimal multiple of n that equals the difference of two primes.
   The algorithm searches multiples of n (n, 2n, 3n, ...) and for each multiple d
   checks if there exists a pair of primes with difference d.  For odd d only
   the pair (2, d+2) is possible; for even d we scan odd primes until we find
   a partner.  The prime sieve is built on demand up to a limit that is
   enlarged if necessary.  Results are memoised.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- prime sieve & helpers ---------- */

static uint8_t   *prime_flag  = NULL;   /* 0 = composite, 1 = prime */
static uint32_t *prime_tbl   = NULL;   /* list of primes */
static size_t    prime_cnt   = 0;
static size_t    prime_cap   = 0;
static size_t    prime_limit = 0;      /* current sieve limit */

/* enlarge the sieve if necessary */
static void extend_primes_to(size_t new_limit)
{
    if (new_limit <= prime_limit)
        return;

    /* free old data */
    free(prime_flag);
    free(prime_tbl);

    prime_limit = new_limit;
    prime_flag  = (uint8_t *)calloc(prime_limit + 1, 1);
    if (!prime_flag) abort();

    /* basic sieve */
    memset(prime_flag, 1, prime_limit + 1);
    prime_flag[0] = prime_flag[1] = 0;
    for (size_t i = 2; i * i <= prime_limit; ++i)
        if (prime_flag[i])
            for (size_t j = i * i; j <= prime_limit; j += i)
                prime_flag[j] = 0;

    /* build prime table */
    prime_cnt = 0;
    prime_cap = 64;
    prime_tbl = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_tbl) abort();

    for (size_t i = 2; i <= prime_limit; ++i)
        if (prime_flag[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(prime_tbl, prime_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_tbl = tmp;
            }
            prime_tbl[prime_cnt++] = (uint32_t)i;
        }
}

/* ---------- caching of results ---------- */

static seq_t *cache   = NULL;
static size_t cache_sz = 0;

/* enlarge cache if needed */
static void ensure_cache(idx_t n)
{
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();
        /* initialise new entries to 0 (indicating "not computed") */
        memset(tmp + cache_sz, 0, (new_sz - cache_sz) * sizeof(seq_t));
        cache      = tmp;
        cache_sz   = new_sz;
    }
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* offset 1 */

    ensure_cache(n);
    if (cache[n] != (seq_t)0)           /* already computed */
        return cache[n];

    /* initialise prime sieve if not yet done */
    if (!prime_flag)
        extend_primes_to(10000000);     /* start with a reasonably large limit */

    seq_t result = 0;
    uint64_t nn = (uint64_t)n;

    for (uint64_t k = 1; ; ++k) {
        uint64_t d64 = nn * k;
        seq_t d = (seq_t)d64;

        /* if d is odd, only possible pair is (2, 2+d) */
        if (d % 2 == 1) {
            uint64_t need = 2ULL + d64;
            if (need > prime_limit)
                extend_primes_to(need * 2);   /* ensure we can test this odd case */
            if (prime_flag[need]) {
                result = d;
                break;
            }
            continue;                          /* try next multiple */
        }

        /* even difference: search odd primes q such that q+d is prime */
        bool found = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t q = prime_tbl[i];
            if ((size_t)q > prime_limit - d64) break;   /* p would exceed current limit */

            uint32_t p = q + (uint32_t)d64;
            if (prime_flag[p]) {
                found = true;
                break;
            }
        }

        if (found) {
            result = d;
            break;
        }

        /* If we didn't find a pair within current limit, enlarge sieve */
        size_t new_lim = prime_limit * 2;
        if (new_lim < d64 * 2)
            new_lim = d64 * 2;
        extend_primes_to(new_lim);
        /* retry this d after expanding */
    }

    cache[n] = result;
    return result;
}
