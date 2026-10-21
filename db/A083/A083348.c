/*
 * A083348  Numbers k such that r(k) = Sum(e/p: k = Product(p^e)) > 1.
 * https://oeis.org/A083348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083348 – Numbers k with r(k) = Σ(e/p) > 1
    (k = Π p^e)                                                  */
#include <stdint.h>
#include <stdlib.h>

/* ---------- One‑time sieve data ------------------------------------- */
static uint32_t *spf          = NULL;   /* smallest prime factor   */
static uint32_t *prime_list   = NULL;   /* list of primes          */
static size_t   prime_count   = 0;      /* current number of primes*/
static size_t   max_sieve     = 0;      /* maximum indexed value    */

/* ---------- One‑time cache for the sequence -------------------------- */
static seq_t   *cache       = NULL;   /* found terms              */
static size_t  cache_len    = 0;      /* current cache size       */
static size_t  cache_cap    = 0;      /* allocated cache capacity */
static idx_t   next_check   = 7;      /* next candidate (starts at 8) */

/* ---------- Utility: gcd for __int128 --------------------------------- */
static inline unsigned __int128
gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b) {
        unsigned __int128 t = a % b;
        a = b;  b = t;
    }
    return a;
}

/* ---------- Build or resize the linear sieve ------------------------ */
static void
build_sieve(size_t limit)
{
    /* Allocate/resize the spf array */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    if (!new_spf) return;                     /* out‑of‑memory – let caller fail */
    spf = new_spf;
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* Allocate an over‑estimation for the primes array */
    size_t prime_est = limit / 10 + 10;
    uint32_t *new_plist = (uint32_t *)realloc(prime_list, prime_est * sizeof(uint32_t));
    if (!new_plist) return;
    prime_list = new_plist;

    /* Linear sieve */
    prime_count = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_count < prime_est) {
                prime_list[prime_count++] = (uint32_t)i;
            }
        }
        for (size_t j = 0; j < prime_count; ++j) {
            uint32_t p = prime_list[j];
            size_t v = i * (size_t)p;
            if (v > limit) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }
    max_sieve = limit;
}

/* ---------- Ensure sieve covers at least n -------------------------------- */
static void
ensure_sieve(size_t n)
{
    if (n > max_sieve) {
        size_t new_lim = n * 2 + 10;  /* safety margin */
        build_sieve(new_lim);
    }
}

/* ---------- Append to cache --------------------------------------------- */
static void
cache_append(idx_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!new_cache) return;                  /* out‑of‑memory – abort silently */
        cache = new_cache;
    }
    cache[cache_len++] = (seq_t)val;
}

/* ---------- The required sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined, but harmless */

    /* Bootstrap the sieve if this is the first call */
    if (max_sieve == 0) build_sieve(100);

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++next_check;                           /* new candidate k */

        /* Ensure the sieve covers this candidate */
        ensure_sieve((size_t)next_check);

        /* Compute r(k) = Σ e/p for k = Π p^e */
        idx_t tmp = next_check;
        unsigned __int128 num = 0;              /* numerator */
        unsigned __int128 den = 1;              /* denominator */

        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            unsigned __int128 new_num = num * p + (unsigned __int128)e * den;
            unsigned __int128 new_den = den * p;
            unsigned __int128 g = gcd128(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }

        if (num > den)            /* condition r(k) > 1 */
            cache_append(next_check);
    }

    return cache[n - 1];            /* sequence is 1‑based */
}
