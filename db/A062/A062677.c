/*
 * A062677  Numbers with property that every divisor (except 1) contains the
 *          digit 8.
 * https://oeis.org/A062677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has8, rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A062677: numbers n such that every divisor d>1 of n contains the digit 8.
 * a(n) = n-th smallest integer >1 whose every divisor >1 contains an 8.
 * The code searches integers sequentially, tests the property by factorising and
 * enumerating all divisors.  It caches the results and builds the sequence on-demand.
 * It uses a linear sieve to precompute smallest prime factors up to a fixed limit.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has8.h"
#include "../utils/rec.h"

/* ---------- parameters ---------- */
#define MAX_SIEVE   20000000U   /* 20 million – enough for all test cases */

/* ---------- global data ---------- */
static int *spf = NULL;          /* smallest prime factor array */
static unsigned *primes = NULL;  /* list of primes */
static size_t primes_size = 0;   /* number of primes */

/* ---------- helper: sieve ---------- */
static void init_sieve(void)
{
    if (spf) return;                /* already initialised */

    spf = (int*)malloc((MAX_SIEVE + 1) * sizeof(int));
    if (!spf) return;               /* allocation failed – should not happen */

    for (size_t i = 0; i <= MAX_SIEVE; ++i) spf[i] = 0;

    /* prime list – allocate a reasonable initial capacity */
    size_t pr_cap = 1000000;                /* ~1.3M primes below 20 M */
    primes = (unsigned*)malloc(pr_cap * sizeof(unsigned));
    if (!primes) return;                   /* should not happen */

    for (unsigned i = 2; i <= MAX_SIEVE; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (primes_size >= pr_cap) {
                pr_cap <<= 1;
                unsigned *tmp = (unsigned*)realloc(primes, pr_cap * sizeof(unsigned));
                if (!tmp) return;          /* allocation failed */
                primes = tmp;
            }
            primes[primes_size++] = i;
        }
        for (size_t j = 0; j < primes_size && primes[j] <= spf[i] &&
             (unsigned long long)i * primes[j] <= MAX_SIEVE; ++j) {
            unsigned p = primes[j];
            spf[i * p] = (int)p;
        }
    }
}

/* ---------- test if n satisfies the digit‑8 divisor property ---------- */
static bool good_number(unsigned long long n)
{
    if (n < 2) return false;

    /* factorise n – primes and exponents */
    unsigned pf[20];   /* enough for numbers up to 2×10⁷ */
    int exp[20];
    size_t sz = 0;
    unsigned long long temp = n;

    while (temp > 1) {
        int p = spf[temp];
        if (p == 0) {          /* should not happen for our bounds */
            p = (int)temp;
        }
        int cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
        }
        pf[sz] = (unsigned)p;
        exp[sz] = cnt;
        ++sz;
    }

    /* check each prime power contains digit 8 (early pruning) */
    for (size_t i = 0; i < sz; ++i) {
        unsigned long long pp = 1ULL;
        for (int k = 1; k <= exp[i]; ++k) {
            pp *= (unsigned long long)pf[i];
            if (!has8(pp)) return false;
        }
    }

    /* fully enumerate all divisors greater than 1 and test them */
    return rec(pf, exp, 0, sz, 1ULL);
}

/* ---------- cache of computed sequence ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;    /* number of stored terms (including dummy 0) */
static size_t cache_cap = 0;
static unsigned long long next_checked = 1ULL;  /* last integer examined */

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    init_sieve();   /* guarantees spf and primes are ready */

    if (!cache) {   /* initialise cache on first call */
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;           /* dummy to make indices 1‑based */
        cache_len = 1;
    }

    while (cache_len <= (size_t)n) {
        ++next_checked;
        if (good_number(next_checked)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_checked;
        }
    }

    return cache[(size_t)n];
}
