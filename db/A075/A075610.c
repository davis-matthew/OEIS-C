/*
 * A075610  a(1) = 1, a(n) = smallest prime number not occurring earlier such
 *          that the concatenation a(n-1) and a(n) is a composite number.
 * https://oeis.org/A075610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Algorithm: a(1) = 1.
 * For n > 1: find the smallest unused prime p such that
 *   concat(a(n-1), p) is a composite (i.e., not prime).
 * The sequence is built lazily; results are memoised so that calls
 * can be in any order.  The implementation uses a simple sieve to
 * generate primes up to a fixed bound large enough for n up to 10 000.
 * Concatenation is performed by multiplying by 10^digits(p).
 * Prime testing uses the supplied isPrimeGeneric() helper.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

/* ---------- local helper functions ---------- */

/* Power of 10 for a given exponent */
static seq_t pow10_idx(idx_t e)
{
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* Number of decimal digits of x (x > 0) */
static int digits_seq(seq_t x)
{
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* ---------- prime storage ---------- */
static uint32_t *prime_raw   = NULL;   /* array of primes (uint32_t)   */
static bool   *prime_used   = NULL;   /* true if prime already used  */
static size_t  prime_count  = 0;       /* number of primes stored     */
static const size_t PRIME_LIMIT = 200000; /* sufficient for n <= 10000 */

/* Build a list of primes up to PRIME_LIMIT using a simple sieve */
static void build_primes(void)
{
    if (prime_count != 0) return;      /* already built */

    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!sieve) return;
    memset(sieve, 1, (PRIME_LIMIT + 1) * sizeof(bool));
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++cnt;

    prime_raw   = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    prime_used  = (bool*)malloc(cnt * sizeof(bool));
    if (!prime_raw || !prime_used) { free(sieve); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) {
            prime_raw[idx]  = (uint32_t)i;
            prime_used[idx] = false;
            ++idx;
        }
    prime_count = cnt;
    free(sieve);
}

/* ---------- sequence cache ---------- */
static seq_t *cache      = NULL;   /* memoised a(n) values */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_size = 0;      /* number of values stored */

/* Grow cache capacity to at least 'need' elements */
static void cache_grow(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (need > 2 * cache_cap) ? need : 2 * cache_cap;
    if (new_cap == 0) new_cap = 16;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp) {
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; 0 is unused */

    if (cache_size == 0) {              /* initialise on first call */
        cache_grow(1);
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        cache_size = 1;
    }

    if ((idx_t)cache_size >= n)           /* already computed */
        return cache[n-1];

    build_primes();                       /* ensure primes ready */

    cache_grow(n);
    for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
        seq_t prev = cache[i-2];
        seq_t chosen = 0;

        for (size_t pi = 0; pi < prime_count; ++pi) {
            if (prime_used[pi]) continue;
            seq_t p = (seq_t)prime_raw[pi];
            int d = digits_seq(p);
            seq_t pow = pow10_idx((idx_t)d);
            seq_t concat = prev * pow + p;
            if (!isPrimeGeneric(concat)) {
                chosen = p;
                prime_used[pi] = true;
                break;
            }
        }

        /* In the unlikely event that we need more primes (beyond 200000),
         * the algorithm would fail here.  For n <= 10 000 the prebuilt
         * list suffices. */
        cache[i-1] = chosen;
    }

    cache_size = n;
    return cache[n-1];
}
