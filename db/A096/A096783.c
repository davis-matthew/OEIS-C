/*
 * A096783  Numbers n such that n, n+1, n+2, n+3, n+4 are all of the form
 *          x^2+2*y^2 for nonnegative x, y.
 * https://oeis.org/A096783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 6922, u64 n <= 6922,
 *            u128 n <= 6922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A096783 – Numbers n such that n, n+1, n+2, n+3, n+4 are all of the
   form x^2 + 2*y^2.  All terms are multiples of 32.  This program
   implements a fast sieve for prime factors up to a fixed limit and
   checks the parity condition on primes ≡ 5,7 (mod 8). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t LIMIT = 2000000ULL;      /* sieve bound, enough for n ≤ 10 000 */
static uint32_t *spf = NULL;                  /* smallest prime factor array */
static uint32_t *primes = NULL;               /* list of primes up to LIMIT */
static size_t primes_len = 0;

static void init_sieve(void)
{
    if (spf != NULL) return;                 /* already initialized */
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    primes = (uint32_t *)malloc(LIMIT * sizeof(uint32_t)); /* upper bound */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {                   /* i is prime */
            spf[i] = (uint32_t)i;
            primes[cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < cnt && (uint64_t)primes[j] <= spf[i]
                      && i * (uint64_t)primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    primes_len = cnt;
}

/* true  ⇔  n can be written as x^2 + 2*y^2  (n≤0 is considered true)   */
static bool rep_ok_uint64(uint64_t n)
{
    if (n == 0) return true;
    /* remove all 2’s – they are unrestricted */
    while ((n & 1ULL) == 0) n >>= 1;
    if (n == 1) return true;
    uint64_t m = n;
    while (m > 1) {
        uint32_t p = 0;
        if (m <= LIMIT) {
            p = spf[m];
        } else {
            /* trial division using pre‑computed primes */
            bool found = false;
            for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= m; ++i) {
                if (m % primes[i] == 0) { p = primes[i]; found = true; break; }
            }
            if (!found) p = (uint32_t)m;      /* m is prime > LIMIT */
        }
        int odd = 0;
        do { m /= p; odd ^= 1; } while (m % p == 0);
        if ((p & 7U) == 5U || (p & 7U) == 7U) {
            if (odd) return false;
        }
    }
    return true;
}

/* dynamic cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;     /* next multiple of 32 to examine */

/* double cache capacity when needed */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (newp) { cache = newp; cache_cap = new_cap; }
}

/* ensure that the cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        init_sieve();
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        cache_cap = 16;
        cache_size = 0;
        last_checked = 0;
    }
    while (cache_size < (size_t)n) {
        uint64_t cand = last_checked;
        /* align to a multiple of 32 */
        if ((cand & 31ULL) != 0) cand += 32ULL - (cand & 31ULL);

        for (;;) {
            bool ok = true;
            for (int offset = 0; offset < 5; ++offset) {
                if (!rep_ok_uint64(cand + offset)) { ok = false; break; }
            }
            if (ok) break;
            cand += 32;
        }
        grow_cache(cache_size + 1);
        cache[cache_size++] = (seq_t)cand;
        last_checked = cand + 32;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety – offset is 1 */
    ensure_cache(n);
    return cache[n - 1];
}
