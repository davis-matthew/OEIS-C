/*
 * A155145  Primes p such that p-+1, p-+3, p-+5 are not squarefree.
 * https://oeis.org/A155145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   BIT_CLEAR, BIT_TEST
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The terms are the primes p for which the six integers p-5, p-3,
 * p-1, p+1, p+3, p+5 are all non‑squarefree.
 * The implementation sieves once up to a fixed LIMIT and
 * stores the sequence in a cache.  On every call it
 * continues the sieve until the requested index is available.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/BIT_CLEAR.h"
#include "../utils/BIT_TEST.h"

/* ----- static state ---------------------------------------------------- */
static bool            initialized   = false;
static unsigned char  *prime_bits    = NULL;   /* primality flag */
static unsigned char  *sqfree_bits   = NULL;   /* square‑free flag */
static idx_t          *cache         = NULL;   /* found primes */
static size_t          cache_cap     = 0;      /* cache capacity */
static size_t          cache_size    = 0;      /* cache used */
static idx_t           scan_pos      = 0;      /* next integer to test */

/* ----- sieve limit ----------------------------------------------------- */
#define LIMIT   200000000ULL   /* 200 million – enough for the first 10000 terms */

/* ----- initialization --------------------------------------------------- */
static void init_sieve(void)
{
    /* allocate and build primality sieve */
    size_t prime_bytes = (LIMIT >> 3) + 1;
    prime_bits = (unsigned char*)malloc(prime_bytes);
    memset(prime_bits, 0xFF, prime_bytes);
    BIT_CLEAR(prime_bits, 0);   /* 0 and 1 are not prime */
    BIT_CLEAR(prime_bits, 1);

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= LIMIT; ++i)
        if (BIT_TEST(prime_bits, i))
            for (idx_t j = i * i; j <= LIMIT; j += i)
                BIT_CLEAR(prime_bits, j);

    /* allocate and build non‑squarefree sieve */
    size_t sqfree_bytes = ((LIMIT + 5) >> 3) + 1;
    sqfree_bits = (unsigned char*)malloc(sqfree_bytes);
    memset(sqfree_bits, 0xFF, sqfree_bytes);   /* assume all square‑free */

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= LIMIT + 5; ++i)
        if (BIT_TEST(prime_bits, i)) {
            idx_t sq = i * i;
            for (idx_t j = sq; j <= LIMIT + 5; j += sq)
                BIT_CLEAR(sqfree_bits, j);     /* mark as NOT square‑free */
        }

    /* initialise cache */
    cache_cap  = 1024;
    cache      = (idx_t*)malloc(cache_cap * sizeof(idx_t));
    cache_size = 0;
    scan_pos   = 2;         /* first integer to examine */
    initialized = true;
}

/* ----- cache helpers --------------------------------------------------- */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap * 2);
    idx_t *tmp = (idx_t*)realloc(cache, new_cap * sizeof(idx_t));
    if (!tmp) abort();        /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}
static void add_to_cache(idx_t val)
{
    if (cache_size == cache_cap) grow_cache();
    cache[cache_size++] = val;
}

/* offsets to test */
static const int offsets[6] = { -5, -3, -1, 1, 3, 5 };

/* ----- main sequence function ----------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – guard clause */

    if (!initialized) init_sieve();

    /* extend the cache until it contains at least n terms */
    while (cache_size < n && scan_pos <= LIMIT - 5) {
        if (BIT_TEST(prime_bits, scan_pos)) {
            bool ok = true;
            for (int k = 0; k < 6; ++k) {
                long long pos = (long long)scan_pos + offsets[k];
                if (pos < 2) { ok = false; break; }          /* out of bound */
                if (pos > (long long)LIMIT + 5) { ok = false; break; }
                if (BIT_TEST(sqfree_bits, (idx_t)pos)) {      /* square‑free */
                    ok = false; break;
                }
            }
            if (ok) add_to_cache(scan_pos);
        }
        ++scan_pos;
    }

    /* if we exhausted the sieve – abort; limit should be large enough */
    if (cache_size < n) abort();

    return (seq_t)cache[n - 1];
}
